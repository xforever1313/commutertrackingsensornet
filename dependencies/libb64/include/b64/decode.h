// :mode=c++:
/*
decode.h - c++ wrapper for a base64 decoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/
#ifndef BASE64_DECODE_H
#define BASE64_DECODE_H

#include <iostream>
#include <string>

namespace base64
{
	extern "C"
	{
		#include "b64/cdecode.h"
	}

	struct decoder
	{
		base64_decodestate _state;
		int _buffersize;

		decoder(int buffersize_in = 255)
		: _buffersize(buffersize_in)
		{}

		int decode(char value_in)
		{
			return base64_decode_value(value_in);
		}

		int decode(const char* code_in, const int length_in, char* plaintext_out)
		{
			return base64_decode_block(code_in, length_in, plaintext_out, &_state);
		}

		void decode(std::istream& istream_in, std::ostream& ostream_in)
		{
			base64_init_decodestate(&_state);
			//
			const int N = _buffersize;
			std::string code(N, '\0');
			std::string plaintext(N, '\0');
			int codelength;
			int plainlength;

			do
			{
				istream_in.read(const_cast<char*>(code.data()), N);
				codelength = istream_in.gcount();
				plainlength = decode(code.data(), codelength, &plaintext[0]);
				ostream_in.write(plaintext.data(), plainlength);
			}
			while (istream_in.good() && codelength > 0);
			//
			base64_init_decodestate(&_state);

		}
	};

} // namespace base64



#endif // BASE64_DECODE_H

