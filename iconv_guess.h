#ifndef _ICONV_GUESS
#define _ICONV_GUESS

#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void iconv_guess_encoding_list(char **encodings, int *encodings_num, int enable_alias);
extern int iconv_guess_is_valid_encoding(const char *encname);
extern void iconv_guess_encoding(const char *from, size_t from_size, const char **encodings, char *guess_enc, int *guess_score, int allowed_score);
extern int iconv_guess_convert(const char *to_encoding, const char **from, size_t *from_size, char **to, size_t *to_size, const char **encodings, int allow_guess_score);

#ifdef __cplusplus
}
#endif

#endif
