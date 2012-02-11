/*
 * Copyright (C) 2012 Yoshida Tetsuya
 * 
 * This file is part of the LIBICONV_GUESS Library.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "iconv_guess.h"


// guess encoding

void iconv_guess_encoding(const char *from, size_t from_size, const char **encodings, char *guessed_enc, int *error_num, int allowed_error_num)
{
	const char *encoding;
	char *p_from;
	size_t p_from_size;
	unsigned int i;

	// iconv
	iconv_t ic;
	size_t ic_ret;
	unsigned int current_error_num;

	// to
	char *to, *p_to;
	size_t to_size, p_to_size;

	// tmp result
	char tmp_guessed_enc[32];
	int min_error_num;
	int tmp_result_flag;

	// init memory
	to = (char*)malloc(from_size);
	to_size = from_size;
	i = 0;
	tmp_guessed_enc[0] = '\0';
	min_error_num = -1;

	while (encodings[i]) {
		// reset ptr
		encoding = encodings[i];
		p_from = (char*)from;
		p_from_size = from_size;
		p_to = to;
		p_to_size = to_size;

		// init iconv
		ic = iconv_open("UCS-4-INTERNAL", encoding);
		current_error_num = 0;
		tmp_result_flag = 1;

		// iconv open error
		if (ic==(iconv_t)-1) {
			tmp_result_flag = 0;
		}

		while (p_from_size && tmp_result_flag) {
			ic_ret = iconv(ic, &p_from, &p_from_size, &p_to, &p_to_size);

			// break: no error
			if ((size_t)-1!=ic_ret) {
				break;
			}

			switch (errno) {
			case EILSEQ:
				// continue: Input conversion stopped due to an input byte that does not belong to the input codeset.
				current_error_num++;
				p_from++;
				p_from_size--;
				break;
			case E2BIG:
				// continue: Input conversion stopped due to lack of space in the output buffer.
				p_to = to;
				p_to_size = to_size;
				break;
			default:
				// break: error
				tmp_result_flag = 0;
				break;
			}

			// break: Exceed the min_error_num
			if (-1<min_error_num && min_error_num<current_error_num) {
				tmp_result_flag = 0;
			}

			// break: Exceed the allowed_error_num.
			if (-1<allowed_error_num && allowed_error_num<current_error_num) {
				tmp_result_flag = 0;
			}
		}
		if (ic!=(iconv_t)-1) {
			iconv_close(ic);
		}

		// update tmp result
		if (tmp_result_flag) {
			strcpy(tmp_guessed_enc, encoding);
			min_error_num = current_error_num;
			if (current_error_num==0) {
				break;
			}
		}
		i++;
	}

	free(to);
	strcpy(guessed_enc, tmp_guessed_enc);
	if (error_num) {
		*error_num = min_error_num;
	}
}


// guess convert

int iconv_guess_convert(const char *to_encoding, const char **from, size_t *from_size, char **to, size_t *to_size, const char **encodings, int allowed_error_num)
{
	char from_encoding[32];
	int error_num;
	iconv_t ic;
	size_t ic_ret;
	int conv_count;
	int result_flag;

	conv_count = 0;
	result_flag = 1;
	iconv_guess_encoding(*from, *from_size, encodings, from_encoding, &error_num, allowed_error_num);

	if (-1<error_num) {
		// use iconv
		//printf("from_encoding: %s\n", from_encoding);
		ic = iconv_open(to_encoding, from_encoding);

		// iconv open error
		if (ic==(iconv_t)-1) {
			result_flag = 0;
		}

		while (from_size && result_flag) {
			ic_ret = iconv(ic, (char**)from, from_size, to, to_size);
			conv_count++;

			// break: no error
			if ((size_t)-1!=ic_ret) {
				break;
			}

			switch (errno) {
			case EILSEQ:
				// continue: Input conversion stopped due to an input byte that does not belong to the input codeset.
				from++;
				from_size--;
				break;
			default:
				// break: error
				result_flag = 0;
				break;
			}
		}
		if (ic!=(iconv_t)-1) {
			iconv_close(ic);
		}
	}
	return conv_count;
}
