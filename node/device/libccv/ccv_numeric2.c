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

void ccv_eigen(ccv_dense_matrix_t* a, ccv_dense_matrix_t** vector, ccv_dense_matrix_t** lambda, int type, double epsilon)
{
	ccv_declare_derived_signature(vsig, a->sig != 0, ccv_sign_with_literal("ccv_eigen(vector)"), a->sig, CCV_EOF_SIGN);
	ccv_declare_derived_signature(lsig, a->sig != 0, ccv_sign_with_literal("ccv_eigen(lambda)"), a->sig, CCV_EOF_SIGN);
	assert(CCV_GET_CHANNEL(a->type) == 1);
	type = (type == 0) ? CCV_GET_DATA_TYPE(a->type) | CCV_C1 : CCV_GET_DATA_TYPE(type) | CCV_C1;
	// as of now, this function only support real symmetric matrix
	ccv_dense_matrix_t* dvector = *vector = ccv_dense_matrix_renew(*vector, a->rows, a->cols, CCV_32F | CCV_64F | CCV_C1, type, vsig);
	ccv_dense_matrix_t* dlambda = *lambda = ccv_dense_matrix_renew(*lambda, 1, a->cols, CCV_32F | CCV_64F | CCV_C1, type, lsig);
	assert(CCV_GET_DATA_TYPE(dvector->type) == CCV_GET_DATA_TYPE(dlambda->type));
	ccv_object_return_if_cached(, dvector, dlambda);
	double* ja = (double*)ccmalloc(sizeof(double) * a->rows * a->cols);
	int i, j;
	unsigned char* aptr = a->data.u8;
	assert(a->rows > 0 && a->cols > 0);
#define for_block(_, _for_get) \
	for (i = 0; i < a->rows; i++) \
	{ \
		for (j = 0; j < a->cols; j++) \
			ja[i * a->cols + j] = _for_get(aptr, j, 0); \
		aptr += a->step; \
	}
	ccv_matrix_getter(a->type, for_block);
#undef for_block
	ccv_zero(dvector);
	ccv_zero(dlambda);
	unsigned char* dvptr = dvector->data.u8;
#define for_block(_, _for_set) \
	for (i = 0; i < a->cols; i++) \
		_for_set(dvptr, i * a->cols + i, 1, 0);
	ccv_matrix_setter(dvector->type, for_block);
#undef for_block
	double accuracy = 0;
	for (i = 0; i < a->rows * a->cols; i++)
		accuracy += ja[i];
	accuracy = sqrt(2 * accuracy);
	int p, q;
	unsigned char* dlptr = dlambda->data.u8;
	int flag = 1;
	assert(a->rows == a->cols);
#define for_block(_, _for_set, _for_get) \
	do { \
		if (!flag) \
			accuracy = accuracy * 0.5; \
		flag = 0; \
		for (p = 0; p < a->rows; p++) \
		{ \
			for (q = p + 1; q < a->cols; q++) \
				if (fabs(ja[p * a->cols + q]) > accuracy) \
				{ \
					double x = -ja[p * a->cols + q]; \
					double y = (ja[q * a->cols + q] - ja[p * a->cols + p]) * 0.5; \
					double omega = (x == 0 && y == 0) ? 1 : x / sqrt(x * x + y * y); \
					if (y < 0) \
						omega = -omega; \
					double sn = 1.0 + sqrt(1.0 - omega * omega); \
					sn = omega / sqrt(2 * sn); \
					double cn = sqrt(1.0 - sn * sn); \
					double fpp = ja[p * a->cols + p]; \
					double fpq = ja[p * a->cols + q]; \
					double fqq = ja[q * a->cols + q]; \
					ja[p * a->cols + p] = fpp * cn * cn + fqq * sn * sn + fpq * omega; \
					ja[q * a->cols + q] = fpp * sn * sn + fqq * cn * cn - fpq * omega; \
					ja[p * a->cols + q] = ja[q * a->cols + p] = 0; \
					for (i = 0; i < a->cols; i++) \
						if (i != q && i != p) \
						{ \
							fpp = ja[p * a->cols + i]; \
							fqq = ja[q * a->cols + i]; \
							ja[p * a->cols + i] = fpp * cn + fqq * sn; \
							ja[q * a->cols + i] = -fpp * sn + fqq * cn; \
						} \
					for (i = 0; i < a->rows; i++) \
						if (i != q && i != p) \
						{ \
							fpp = ja[i * a->cols + p]; \
							fqq = ja[i * a->cols + q]; \
							ja[i * a->cols + p] = fpp * cn + fqq * sn; \
							ja[i * a->cols + q] = -fpp * sn + fqq * cn; \
						} \
					for (i = 0; i < a->cols; i++) \
					{ \
						fpp = _for_get(dvptr, p * a->cols + i, 0); \
						fqq = _for_get(dvptr, q * a->cols + i, 0); \
						_for_set(dvptr, p * a->cols + i, fpp * cn + fqq * sn, 0); \
						_for_set(dvptr, q * a->cols + i, -fpp * sn + fqq * cn, 0); \
					} \
					for (i = 0; i < a->cols; i++) \
						_for_set(dlptr, i, ja[i * a->cols + i], 0); \
					flag = 1; \
					break; \
				} \
			if (flag) \
				break; \
		} \
	} while (accuracy > epsilon);
	ccv_matrix_setter_getter(dvector->type, for_block);
#undef for_block
	ccfree(ja);
}

void ccv_minimize(ccv_dense_matrix_t* x, int length, double red, ccv_minimize_f func, ccv_minimize_param_t params, void* data)
{
	ccv_dense_matrix_t* df0 = ccv_dense_matrix_new(x->rows, x->cols, x->type, 0, 0);
	ccv_zero(df0);
	ccv_dense_matrix_t* df3 = ccv_dense_matrix_new(x->rows, x->cols, x->type, 0, 0);
	ccv_zero(df3);
	ccv_dense_matrix_t* dF0 = ccv_dense_matrix_new(x->rows, x->cols, x->type, 0, 0);
	ccv_zero(dF0);
	ccv_dense_matrix_t* s = ccv_dense_matrix_new(x->rows, x->cols, x->type, 0, 0);
	ccv_zero(s);
	ccv_dense_matrix_t* x0 = ccv_dense_matrix_new(x->rows, x->cols, x->type, 0, 0);
	ccv_zero(x0);
	ccv_dense_matrix_t* xn = ccv_dense_matrix_new(x->rows, x->cols, x->type, 0, 0);
	ccv_zero(xn);
	
	double F0 = 0, f0 = 0, f1 = 0, f2 = 0, f3 = 0, f4 = 0;
	double x1 = 0, x2 = 0, x3 = 0, x4 = 0;
	double d0 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0;
	double A = 0, B = 0;
	int ls_failed = 0;

	int i, j, k;
	func(x, &f0, df0, data);
	d0 = 0;
	unsigned char* df0p = df0->data.u8;
	unsigned char* sp = s->data.u8;
	for (i = 0; i < x->rows; i++)
	{
		for (j = 0; j < x->cols; j++)
		{
			double ss = ccv_get_value(x->type, df0p, j);
			ccv_set_value(x->type, sp, j, -ss, 0);
			d0 += -ss * ss;
		}
		df0p += x->step;
		sp += x->step;
	}
	x3 = red / (1.0 - d0);
	int l = (length > 0) ? length : -length;
	int ls = (length > 0) ? 1 : 0;
	int eh = (length > 0) ? 0 : 1;
	for (k = 0; k < l;)
	{
		k += ls;
		memcpy(x0->data.u8, x->data.u8, x->rows * x->step);
		memcpy(dF0->data.u8, df0->data.u8, x->rows * x->step);
		F0 = f0;
		int m = ccv_min(params.max_iter, (length > 0) ? params.max_iter : l - k);
		for (;;)
		{
			x2 = 0;
			f3 = f2 = f0;
			d2 = d0;
			memcpy(df3->data.u8, df0->data.u8, x->rows * x->step);
			while (m > 0)
			{
				m--;
				k += eh;
				unsigned char* sp = s->data.u8;
				unsigned char* xp = x->data.u8;
				unsigned char* xnp = xn->data.u8;
				for (i = 0; i < x->rows; i++)
				{
					for (j = 0; j < x->cols; j++)
						ccv_set_value(x->type, xnp, j, x3 * ccv_get_value(x->type, sp, j) + ccv_get_value(x->type, xp, j), 0);
					sp += x->step;
					xp += x->step;
					xnp += x->step;
				}
				if (func(xn, &f3, df3, data) == 0)
					break;
				else
					x3 = (x2 + x3) * 0.5;
			}
			if (f3 < F0)
			{
				memcpy(x0->data.u8, xn->data.u8, x->rows * x->step);
				memcpy(dF0->data.u8, df3->data.u8, x->rows * x->step);
				F0 = f3;
			}
			d3 = 0;
			unsigned char* df3p = df3->data.u8;
			unsigned char* sp = s->data.u8;
			for (i = 0; i < x->rows; i++)
			{
				for (j = 0; j < x->cols; j++)
					d3 += ccv_get_value(x->type, df3p, j) * ccv_get_value(x->type, sp, j);
				df3p += x->step;
				sp += x->step;
			}
			if ((d3 > params.sig * d0) || (f3 > f0 + x3 * params.rho * d0) || (m <= 0))
				break;
			x1 = x2; f1 = f2; d1 = d2;
			x2 = x3; f2 = f3; d2 = d3;
			A = 6.0 * (f1 - f2) + 3.0 * (d2 + d1) * (x2 - x1);
			B = 3.0 * (f2 - f1) - (2.0 * d1 + d2) * (x2 - x1);
			x3 = B * B - A * d1 * (x2 - x1);
			if (x3 < 0)
				x3 = x2 * params.extrap;
			else {
				x3 = x1 - d1 * (x2 - x1) * (x2 - x1) / (B + sqrt(x3));
				if (x3 < 0)
					x3 = x2 * params.extrap;
				else {
					if (x3 > x2 * params.extrap)
						x3 = x2 * params.extrap;
					else if (x3 < x2 + params.interp * (x2 - x1))
						x3 = x2 + params.interp * (x2 - x1);
				}
			}
		}
		while (((fabs(d3) > -params.sig * d0) || (f3 > f0 + x3 * params.rho * d0)) && (m > 0))
		{
			if ((d3 > 1e-8) || (f3 > f0 + x3 * params.rho * d0))
			{
				x4 = x3; f4 = f3; d4 = d3;
			} else {
				x2 = x3; f2 = f3; d2 = d3;
			}
			if (f4 > f0)
				x3 = x2 - (0.5 * d2 * (x4 - x2) * (x4 - x2)) / (f4 - f2 - d2 * (x4 - x2));
			else {
				A = 6.0 * (f2 - f4) / (x4 - x2) + 3.0 * (d4 + d2);
				B = 3.0 * (f4 - f2) - (2.0 * d2 + d4) * (x4 - x2);
				x3 = B * B - A * d2 * (x4 - x2) * (x4 - x2);
				x3 = (x3 < 0) ? (x2 + x4) * 0.5 : x2 + (sqrt(x3) - B) / A;
			}
			x3 = ccv_max(ccv_min(x3, x4 - params.interp * (x4 - x2)), x2 + params.interp * (x4 - x2));
			sp = s->data.u8;
			unsigned char* xp = x->data.u8;
			unsigned char* xnp = xn->data.u8;
			for (i = 0; i < x->rows; i++)
			{
				for (j = 0; j < x->cols; j++)
					ccv_set_value(x->type, xnp, j, x3 * ccv_get_value(x->type, sp, j) + ccv_get_value(x->type, xp, j), 0);
				sp += x->step;
				xp += x->step;
				xnp += x->step;
			}
			func(xn, &f3, df3, data);
			if (f3 < F0)
			{
				memcpy(x0->data.u8, xn->data.u8, x->rows * x->step);
				memcpy(dF0->data.u8, df3->data.u8, x->rows * x->step);
				F0 = f3;
			}
			m--;
			k += eh;
			d3 = 0;
			sp = s->data.u8;
			unsigned char* df3p = df3->data.u8;
			for (i = 0; i < x->rows; i++)
			{
				for (j = 0; j < x->cols; j++)
					d3 += ccv_get_value(x->type, df3p, j) * ccv_get_value(x->type, sp, j);
				df3p += x->step;
				sp += x->step;
			}
		}
		if ((fabs(d3) < -params.sig * d0) && (f3 < f0 + x3 * params.rho * d0))
		{
			memcpy(x->data.u8, xn->data.u8, x->rows * x->step);
			f0 = f3;
			double df0_df3 = 0;
			double df3_df3 = 0;
			double df0_df0 = 0;
			unsigned char* df0p = df0->data.u8;
			unsigned char* df3p = df3->data.u8;
			for (i = 0; i < x->rows; i++)
			{
				for (j = 0; j < x->cols; j++)
				{
					df0_df0 += ccv_get_value(x->type, df0p, j) * ccv_get_value(x->type, df0p, j);
					df0_df3 += ccv_get_value(x->type, df0p, j) * ccv_get_value(x->type, df3p, j);
					df3_df3 += ccv_get_value(x->type, df3p, j) * ccv_get_value(x->type, df3p, j);
				}
				df0p += x->step;
				df3p += x->step;
			}
			double slr = (df3_df3 - df0_df3) / df0_df0;
			df3p = df3->data.u8;
			unsigned char* sp = s->data.u8;
			for (i = 0; i < x->rows; i++)
			{
				for (j = 0; j < x->cols; j++)
					ccv_set_value(x->type, sp, j, slr * ccv_get_value(x->type, sp, j) - ccv_get_value(x->type, df3p, j), 0);
				df3p += x->step;
				sp += x->step;
			}
			memcpy(df0->data.u8, df3->data.u8, x->rows * x->step);
			d3 = d0;
			d0 = 0;
			df0p = df0->data.u8;
			sp = s->data.u8;
			for (i = 0; i < x->rows; i++)
			{
				for (j = 0; j < x->cols; j++)
				{
					d0 += ccv_get_value(x->type, df0p, j) * ccv_get_value(x->type, sp, j);
				}
				df0p += x->step;
				sp += x->step;
			}
			if (d0 > 0)
			{
				d0 = 0;
				df0p = df0->data.u8;
				sp = s->data.u8;
				for (i = 0; i < x->rows; i++)
				{
					for (j = 0; j < x->cols; j++)
					{
						double ss = ccv_get_value(x->type, df0p, j);
						ccv_set_value(x->type, sp, j, -ss, 0);
						d0 += -ss * ss;
					}
					df0p += x->step;
					sp += x->step;
				}
			}
			x3 = x3 * ccv_min(params.ratio, d3 / (d0 - 1e-8));
			ls_failed = 0;
		} else {
			memcpy(x->data.u8, x0->data.u8, x->rows * x->step);
			memcpy(df0->data.u8, dF0->data.u8, x->rows * x->step);
			f0 = F0;
			if (ls_failed)
				break;
			d0 = 0;
			unsigned char* df0p = df0->data.u8;
			unsigned char* sp = s->data.u8;
			for (i = 0; i < x->rows; i++)
			{
				for (j = 0; j < x->cols; j++)
				{
					double ss = ccv_get_value(x->type, df0p, j);
					ccv_set_value(x->type, sp, j, -ss, 0);
					d0 += -ss * ss;
				}
				df0p += x->step;
				sp += x->step;
			}
			x3 = red / (1.0 - d0);
			ls_failed = 1;
		}
	}
	ccv_matrix_free(s);
	ccv_matrix_free(x0);
	ccv_matrix_free(xn);
	ccv_matrix_free(dF0);
	ccv_matrix_free(df0);
	ccv_matrix_free(df3);
}


void ccv_distance_transform(ccv_dense_matrix_t* a, ccv_dense_matrix_t** b, int type, ccv_dense_matrix_t** x, int x_type, ccv_dense_matrix_t** y, int y_type, double dx, double dy, double dxx, double dyy, int flag)
{
	assert(!(flag & CCV_L2_NORM) && (flag & CCV_GSEDT));
	ccv_declare_derived_signature(sig, a->sig != 0, ccv_sign_with_format(64, "ccv_distance_transform(%la,%la,%la,%la,%d)", dx, dy, dxx, dyy, flag), a->sig, CCV_EOF_SIGN);
	type = (CCV_GET_DATA_TYPE(type) == CCV_64F || CCV_GET_DATA_TYPE(a->type) == CCV_64F || CCV_GET_DATA_TYPE(a->type) == CCV_64S) ? CCV_GET_CHANNEL(a->type) | CCV_64F : CCV_GET_CHANNEL(a->type) | CCV_32F;
	ccv_dense_matrix_t* db = *b = ccv_dense_matrix_renew(*b, a->rows, a->cols, CCV_ALL_DATA_TYPE | CCV_GET_CHANNEL(a->type), type, sig);
	ccv_dense_matrix_t* mx = 0;
	ccv_dense_matrix_t* my = 0;
	if (x != 0)
	{
		ccv_declare_derived_signature(xsig, a->sig != 0, ccv_sign_with_format(64, "ccv_distance_transform_x(%la,%la,%la,%la,%d)", dx, dy, dxx, dyy, flag), a->sig, CCV_EOF_SIGN);
		mx = *x = ccv_dense_matrix_renew(*x, a->rows, a->cols, CCV_32S | CCV_C1, CCV_32S | CCV_C1, xsig);
	}
	if (y != 0)
	{
		ccv_declare_derived_signature(ysig, a->sig != 0, ccv_sign_with_format(64, "ccv_distance_transform_y(%la,%la,%la,%la,%d)", dx, dy, dxx, dyy, flag), a->sig, CCV_EOF_SIGN);
		my = *y = ccv_dense_matrix_renew(*y, a->rows, a->cols, CCV_32S | CCV_C1, CCV_32S | CCV_C1, ysig);
	}
	ccv_object_return_if_cached(, db, mx, my);
	ccv_revive_object_if_cached(db, mx, my);
	int i, j, k;
	unsigned char* a_ptr = a->data.u8;
	unsigned char* b_ptr = db->data.u8;
	int* v = (int*)alloca(sizeof(int) * ccv_max(db->rows, db->cols));
	unsigned char* c_ptr = (unsigned char*)alloca(CCV_GET_DATA_TYPE_SIZE(db->type) * db->rows);
	int* x_ptr = mx ? mx->data.i32 : 0;
	int* y_ptr = my ? my->data.i32 : 0;
#define for_block(_for_max, _for_type_b, _for_set_b, _for_get_b, _for_get_a) \
	_for_type_b _dx = dx, _dy = dy, _dxx = dxx, _dyy = dyy; \
	_for_type_b* z = (_for_type_b*)alloca(sizeof(_for_type_b) * (ccv_max(db->rows, db->cols) + 1)); \
	if (_dxx > 1e-6) \
	{ \
		for (i = 0; i < a->rows; i++) \
		{ \
			k = 0; \
			v[0] = 0; \
			z[0] = (_for_type_b)-_for_max; \
			z[1] = (_for_type_b)_for_max; \
			for (j = 1; j < a->cols; j++) \
			{ \
				_for_type_b s; \
				for (;;) \
				{ \
					assert(k >= 0 && k < ccv_max(db->rows, db->cols)); \
					s = ((SGN _for_get_a(a_ptr, j, 0) + _dxx * j * j - _dx * j) - (SGN _for_get_a(a_ptr, v[k], 0) + _dxx * v[k] * v[k] - _dx * v[k])) / (2.0 * _dxx * (j - v[k])); \
					if (s > z[k]) break; \
					--k; \
				} \
				++k; \
				assert(k >= 0 && k < ccv_max(db->rows, db->cols)); \
				v[k] = j; \
				z[k] = s; \
				z[k + 1] = (_for_type_b)_for_max; \
			} \
			assert(z[k + 1] >= a->cols - 1); \
			k = 0; \
			if (mx) \
			{ \
				for (j = 0; j < a->cols; j++) \
				{ \
					while (z[k + 1] < j) \
					{ \
						assert(k >= 0 && k < ccv_max(db->rows, db->cols) - 1); \
						++k; \
					} \
					_for_set_b(b_ptr, j, _dx * (j - v[k]) + _dxx * (j - v[k]) * (j - v[k]) SGN _for_get_a(a_ptr, v[k], 0), 0); \
					x_ptr[j] = j - v[k]; \
				} \
				x_ptr += mx->cols; \
			} else { \
				for (j = 0; j < a->cols; j++) \
				{ \
					while (z[k + 1] < j) \
					{ \
						assert(k >= 0 && k < ccv_max(db->rows, db->cols) - 1); \
						++k; \
					} \
					_for_set_b(b_ptr, j, _dx * (j - v[k]) + _dxx * (j - v[k]) * (j - v[k]) SGN _for_get_a(a_ptr, v[k], 0), 0); \
				} \
			} \
			a_ptr += a->step; \
			b_ptr += db->step; \
		} \
	} else { /* above algorithm cannot handle dxx == 0 properly, below is special casing for that */ \
		assert(mx == 0); \
		for (i = 0; i < a->rows; i++) \
		{ \
			for (j = 0; j < a->cols; j++) \
				_for_set_b(b_ptr, j, SGN _for_get_a(a_ptr, j, 0), 0); \
			for (j = 1; j < a->cols; j++) \
				_for_set_b(b_ptr, j, ccv_min(_for_get_b(b_ptr, j, 0), _for_get_b(b_ptr, j - 1, 0) + _dx), 0); \
			for (j = a->cols - 2; j >= 0; j--) \
				_for_set_b(b_ptr, j, ccv_min(_for_get_b(b_ptr, j, 0), _for_get_b(b_ptr, j + 1, 0) - _dx), 0); \
			a_ptr += a->step; \
			b_ptr += db->step; \
		} \
	} \
	b_ptr = db->data.u8; \
	if (_dyy > 1e-6) \
	{ \
		for (j = 0; j < db->cols; j++) \
		{ \
			for (i = 0; i < db->rows; i++) \
				_for_set_b(c_ptr, i, _for_get_b(b_ptr + i * db->step, j, 0), 0); \
			k = 0; \
			v[0] = 0; \
			z[0] = (_for_type_b)-_for_max; \
			z[1] = (_for_type_b)_for_max; \
			for (i = 1; i < db->rows; i++) \
			{ \
				_for_type_b s; \
				for (;;) \
				{ \
					assert(k >= 0 && k < ccv_max(db->rows, db->cols)); \
					s = ((_for_get_b(c_ptr, i, 0) + _dyy * i * i - _dy * i) - (_for_get_b(c_ptr, v[k], 0) + _dyy * v[k] * v[k] - _dy * v[k])) / (2.0 * _dyy * (i - v[k])); \
					if (s > z[k]) break; \
					--k; \
				} \
				++k; \
				assert(k >= 0 && k < ccv_max(db->rows, db->cols)); \
				v[k] = i; \
				z[k] = s; \
				z[k + 1] = (_for_type_b)_for_max; \
			} \
			assert(z[k + 1] >= db->rows - 1); \
			k = 0; \
			if (my) \
			{ \
				for (i = 0; i < db->rows; i++) \
				{ \
					while (z[k + 1] < i) \
					{ \
						assert(k >= 0 && k < ccv_max(db->rows, db->cols) - 1); \
						++k; \
					} \
					_for_set_b(b_ptr + i * db->step, j, _dy * (i - v[k]) + _dyy * (i - v[k]) * (i - v[k]) + _for_get_b(c_ptr, v[k], 0), 0); \
					y_ptr[i * my->cols] = i - v[k]; \
				} \
				++y_ptr; \
			} else { \
				for (i = 0; i < db->rows; i++) \
				{ \
					while (z[k + 1] < i) \
					{ \
						assert(k >= 0 && k < ccv_max(db->rows, db->cols) - 1); \
						++k; \
					} \
					_for_set_b(b_ptr + i * db->step, j, _dy * (i - v[k]) + _dyy * (i - v[k]) * (i - v[k]) + _for_get_b(c_ptr, v[k], 0), 0); \
				} \
			} \
		} \
	} else { \
		assert(my == 0); \
		for (j = 0; j < db->cols; j++) \
		{ \
			for (i = 1; i < db->rows; i++) \
				_for_set_b(b_ptr + i * db->step, j, ccv_min(_for_get_b(b_ptr + i * db->step, j, 0), _for_get_b(b_ptr + (i - 1) * db->step, j, 0) + _dy), 0); \
			for (i = db->rows - 2; i >= 0; i--) \
				_for_set_b(b_ptr + i * db->step, j, ccv_min(_for_get_b(b_ptr + i * db->step, j, 0), _for_get_b(b_ptr + (i + 1) * db->step, j, 0) - _dy), 0); \
		} \
	}
	if (flag & CCV_NEGATIVE)
	{
#define SGN -
		if (db->type & CCV_64F)
		{
			ccv_matrix_typeof_setter_getter(db->type, ccv_matrix_getter, a->type, for_block, DBL_MAX);
		} else {
			ccv_matrix_typeof_setter_getter(db->type, ccv_matrix_getter, a->type, for_block, FLT_MAX);
		}
#undef SGN
	} else {
#define SGN +
		if (db->type & CCV_64F)
		{
			ccv_matrix_typeof_setter_getter(db->type, ccv_matrix_getter, a->type, for_block, DBL_MAX);
		} else {
			ccv_matrix_typeof_setter_getter(db->type, ccv_matrix_getter, a->type, for_block, FLT_MAX);
		}
#undef SGN
	}
#undef for_block
}
