// :mode=c++:
/*
encode.h - c++ wrapper for a base64 encoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/
#ifndef BASE64_ENCODE_H
#define BASE64_ENCODE_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace base64
{
	extern "C"
	{
		#include "b64/cencode.h"
	}

	struct encoder
	{
		base64_encodestate _state;
		int _buffersize;

		encoder(int buffersize_in = 255)
		: _buffersize(buffersize_in)
		{}

		int encode(char value_in)
		{
			return base64_encode_value(value_in);
		}

		int encode(const char* code_in, const int length_in, char* plaintext_out)
		{
			return base64_encode_block(code_in, length_in, plaintext_out, &_state);
		}

		int encode_end(char* plaintext_out)
		{
			return base64_encode_blockend(plaintext_out, &_state);
		}

		void encode(std::istream& istream_in, std::ostream& ostream_in)
		{
			base64_init_encodestate(&_state);
			//
			const int N = _buffersize;
			std::string plaintext (N, '\0');
			std::string code (2*N, '\0');
			int plainlength;
			int codelength;

			do
			{
				istream_in.read(&plaintext[0], N);
				plainlength = istream_in.gcount();
				//
				codelength = encode(plaintext.data(), plainlength, &code[0]);
				ostream_in.write(&code[0], codelength);
			}
			while (istream_in.good() && plainlength > 0);

			codelength = encode_end(&code[0]);
			ostream_in.write(&code[0], codelength);
			//
			base64_init_encodestate(&_state);
		}

        const std::string encode(const std::vector<uint8_t> &dataToEncode) {
            base64_init_encodestate(&_state);

            std::string encodedString (dataToEncode.size() * 2, '.');
            int length = encode(reinterpret_cast<char*>(
                                 const_cast<uint8_t*>(dataToEncode.data())),
                                dataToEncode.size(),
                                &encodedString[0]);

            encode_end(&encodedString[length]);

            base64_init_encodestate(&_state);

            return encodedString.substr(0, encodedString.find('.'));
        }
	};

} // namespace base64

#endif // BASE64_ENCODE_H

