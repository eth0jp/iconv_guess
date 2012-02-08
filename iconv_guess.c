#include "iconv_guess.h"


// guess encoding

void iconv_guess_encoding(const char *from, size_t from_size, const char **encodings, char *guess_enc, int *guess_score, int allowed_score)
{
	const char *encoding;
	char *p_from;
	size_t p_from_size;
	unsigned int i;

	// iconv
	iconv_t ic;
	size_t ic_ret;
	unsigned int current_guess_score;

	// to
	char *to, *p_to;
	size_t to_size, p_to_size;

	// tmp result
	char tmp_guess_enc[32];
	int min_guess_score;
	int tmp_result_flag;

	// init memory
	to = (char*)malloc(from_size);
	to_size = from_size;
	i = 0;
	tmp_guess_enc[0] = '\0';
	min_guess_score = -1;

	while (encodings[i]) {
		// reset ptr
		encoding = encodings[i];
		p_from = (char*)from;
		p_from_size = from_size;
		p_to = to;
		p_to_size = to_size;

		// init iconv
		ic = iconv_open("UTF-8", encoding);
		current_guess_score = 0;
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
				current_guess_score++;
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

			// break: Exceed the min_guess_score
			if (-1<min_guess_score && min_guess_score<current_guess_score) {
				tmp_result_flag = 0;
			}

			// break: Exceed the allowed_score.
			if (-1<allowed_score && allowed_score<current_guess_score) {
				tmp_result_flag = 0;
			}
		}
		if (ic!=(iconv_t)-1) {
			iconv_close(ic);
		}

		// update tmp result
		if (tmp_result_flag) {
			strcpy(tmp_guess_enc, encoding);
			min_guess_score = current_guess_score;
			if (current_guess_score==0) {
				break;
			}
		}
		i++;
	}

	free(to);
	strcpy(guess_enc, tmp_guess_enc);
	if (guess_score) {
		*guess_score = min_guess_score;
	}
}


// guess convert

int iconv_guess_convert(const char *to_encoding, const char **from, size_t *from_size, char **to, size_t *to_size, const char **encodings, int allow_guess_score)
{
	char from_encoding[32];
	int score;
	iconv_t ic;
	size_t cp_size;
	size_t ic_ret;
	int conv_count;
	int result_flag;

	conv_count = 0;
	result_flag = 1;
	iconv_guess_encoding(*from, *from_size, encodings, from_encoding, &score, allow_guess_score);

	if (-1<score) {
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
