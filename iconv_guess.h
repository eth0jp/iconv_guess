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

#ifndef _LIBICONV_GUESS_H
#define _LIBICONV_GUESS_H

#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * iconv_guess_encoding
 *
 * Guess encoding.
 *
 * Parameters:
 *  const char *from       : Specifies the target charset.
 *  size_t from_size       : Specifies size of target charset.
 *  const char **encodings : Specifies encoding list to check. Former encoding precedes latter encoding. The smaller a number of specifies, the more precise.
 *  char *guessed_enc      : Guessed encoding.
 *  int *error_num         : The number of error. Return -1 if failed to guess encoding.
 *  int allowed_error_num  : Specifies the allowed number of error. Break a guessing if exceed the allowed_error_num.
 *
 * Return:
 *  void
 */
extern void iconv_guess_encoding(const char *from, size_t from_size, const char **encodings, char *guessed_enc, int *error_num, int allowed_error_num);

/*
 * iconv_guess_convert
 *
 * Parameters:
 *  const char *to_encoding : Specifies the destination encoding.
 *  const char **from       : Specifies the pointer of source charset.
 *  size_t *from_size       : Specifies the size of source charset.
 *  char **to               : Specifies the pointer of destination charset.
 *  size_t *to_size         : Specifies the size of destination charset.
 *  const char **encodings  : Specifies encoding list to check. Former encoding precedes latter encoding.
 *  int allowed_error_num   : Specifies the allowed number of error. Break a guessing if exceed the allowed_error_num.
 *
 * Return:
 *  int : The number of times that the iconv converted to. Return 0 if failed to guess encoding.
 *
 */
extern int iconv_guess_convert(const char *to_encoding, const char **from, size_t *from_size, char **to, size_t *to_size, const char **encodings, int allowed_error_num);

#ifdef __cplusplus
}
#endif

#endif
