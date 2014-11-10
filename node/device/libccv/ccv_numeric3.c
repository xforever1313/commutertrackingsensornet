#include "ccv.h"
#include "ccv_internal.h"
#include <complex.h>
#ifdef HAVE_FFTW3
#include <pthread.h>
#include <fftw3.h>
#else
#include "3rdparty/kissfft/kiss_fftndr.h"
#include "3rdparty/kissfft/kissf_fftndr.h"
#endif

void ccv_invert(ccv_matrix_t* a, ccv_matrix_t** b, int type)
{
}

void ccv_solve(ccv_matrix_t* a, ccv_matrix_t* b, ccv_matrix_t** d, int type)
{
}

void _ccv_filter_direct_8u(ccv_dense_matrix_t* a, ccv_dense_matrix_t* b, ccv_dense_matrix_t* d, int padding_pattern)
{
	int i, j, y, x, k;
	int nz = b->rows * b->cols;
	int* coeff = (int*)ccmalloc(nz * sizeof(int));
	int* cx = (int*)ccmalloc(nz * sizeof(int));
	int* cy = (int*)ccmalloc(nz * sizeof(int));
	int scale = 1 << 14;
	nz = 0;
	for (i = 0; i < b->rows; i++)
		for (j = 0; j < b->cols; j++)
		{
			coeff[nz] = (int)(ccv_get_dense_matrix_cell_value(b, i, j, 0) * scale + 0.5);
			if (coeff[nz] == 0)
				continue;
			cy[nz] = i;
			cx[nz] = j;
			nz++;
		}
	ccv_dense_matrix_t* pa = ccv_dense_matrix_new(a->rows + b->rows / 2 * 2, a->cols + b->cols / 2 * 2, CCV_8U | CCV_C1, 0, 0);
	/* the padding pattern is different from FFT: |aa{BORDER}|abcd|{BORDER}dd| */
	for (i = 0; i < pa->rows; i++)
		for (j = 0; j < pa->cols; j++)
			pa->data.u8[i * pa->step + j] = a->data.u8[ccv_clamp(i - b->rows / 2, 0, a->rows - 1) * a->step + ccv_clamp(j - b->cols / 2, 0, a->cols - 1)];
	unsigned char* m_ptr = d->data.u8;
	unsigned char* a_ptr = pa->data.u8;
	/* 0.5 denote the overhead for indexing x and y */
	if (nz < b->rows * b->cols * 0.75)
	{
		for (i = 0; i < d->rows; i++)
		{
			for (j = 0; j < d->cols; j++)
			{
				int z = 0;
				for (k = 0; k < nz; k++)
					z += a_ptr[cy[k] * pa->step + j + cx[k]] * coeff[k];
				m_ptr[j] = ccv_clamp(z >> 14, 0, 255);
			}
			m_ptr += d->step;
			a_ptr += pa->step;
		}
	} else {
		k = 0;
		for (i = 0; i < b->rows; i++)
			for (j = 0; j < b->cols; j++)
			{
				coeff[k] = (int)(ccv_get_dense_matrix_cell_value(b, i, j, 0) * scale + 0.5);
				k++;
			}
		for (i = 0; i < d->rows; i++)
		{
			for (j = 0; j < d->cols; j++)
			{
				int* c_ptr = coeff;
				int z = 0;
				for (y = 0; y < b->rows; y++)
				{
					int iyx = y * pa->step;
					for (x = 0; x < b->cols; x++)
					{
						z += a_ptr[iyx + j + x] * c_ptr[0];
						c_ptr++;
					}
				}
				m_ptr[j] = ccv_clamp(z >> 14, 0, 255);
			}
			m_ptr += d->step;
			a_ptr += pa->step;
		}
	}
	ccv_matrix_free(pa);
	ccfree(coeff);
	ccfree(cx);
	ccfree(cy);
}

void ccv_filter(ccv_dense_matrix_t* a, ccv_dense_matrix_t* b, ccv_dense_matrix_t** d, int type, int padding_pattern)
{
	ccv_declare_derived_signature(sig, a->sig != 0 && b->sig != 0, ccv_sign_with_literal("ccv_filter"), a->sig, b->sig, CCV_EOF_SIGN);
	type = (type == 0) ? CCV_GET_DATA_TYPE(a->type) | CCV_GET_CHANNEL(a->type) : CCV_GET_DATA_TYPE(type) | CCV_GET_CHANNEL(a->type);
	ccv_dense_matrix_t* dd = *d = ccv_dense_matrix_renew(*d, a->rows, a->cols, CCV_ALL_DATA_TYPE | CCV_GET_CHANNEL(a->type), type, sig);
	ccv_object_return_if_cached(, dd);

	/* 15 is the constant to indicate the high cost of FFT (even with O(nlog(m)) for
	 * integer image.
	 * NOTE: FFT has time complexity of O(nlog(n)), however, for convolution, it
	 * is not the case. Convolving one image (a) to a kernel (b), can be done by
	 * dividing image a to several blocks proportional to (b). Thus, one don't need
	 * to do FFT for the whole image. The image can be divided to n/m part, and
	 * the FFT itself is O(mlog(m)), so, the convolution process has time complexity
	 * of O(nlog(m)) */
	if ((b->rows * b->cols < (log((double)(b->rows * b->cols)) + 1) * 15) && (a->type & CCV_8U))
	{
		_ccv_filter_direct_8u(a, b, dd, padding_pattern);
	} else {
#ifdef HAVE_FFTW3
		_ccv_filter_fftw(a, b, dd, padding_pattern);
#else
		_ccv_filter_kissfft(a, b, dd, padding_pattern);
#endif
	}
}

void ccv_filter_kernel(ccv_dense_matrix_t* x, ccv_filter_kernel_f func, void* data)
{
	int i, j, k, ch = CCV_GET_CHANNEL(x->type);
	unsigned char* m_ptr = x->data.u8;
	double rows_2 = (x->rows - 1) * 0.5;
	double cols_2 = (x->cols - 1) * 0.5;
#define for_block(_, _for_set) \
	for (i = 0; i < x->rows; i++) \
	{ \
		for (j = 0; j < x->cols; j++) \
		{ \
			double result = func(j - cols_2, i - rows_2, data); \
			for (k = 0; k < ch; k++) \
				_for_set(m_ptr, j * ch + k, result, 0); \
		} \
		m_ptr += x->step; \
	}
	ccv_matrix_setter(x->type, for_block);
#undef for_block
	ccv_make_matrix_immutable(x);
}
