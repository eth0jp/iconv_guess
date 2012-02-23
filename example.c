#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iconv_guess.h"

#define BUF_SIZE 10000

/*
Usage: cat txt/sjis.txt | ./example
*/

int main(void)
{
	const char *encodings[] = {"EUC-JP", "SJIS", "UTF-8", NULL};
	char from[BUF_SIZE];
	char *p_from;
	size_t from_size, p_from_size;
	char to[BUF_SIZE];
	char *p_to;
	size_t to_size, p_to_size;
	char enc[32];

	fgets(from, BUF_SIZE, stdin);

	p_from = from;
	p_to = to;
	to_size = p_to_size = BUF_SIZE;
	from_size = p_from_size = strlen(from);

	if (iconv_guess_convert("UTF-8", &p_from, &p_from_size, &p_to, &p_to_size, encodings, enc, NULL, 0)==0) {
		printf("encoding is not found\n");
		return 1;
	}
	printf("%s: %s\n", enc, to);

	return 0;
}
