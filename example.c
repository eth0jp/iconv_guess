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
	const char *p_from;
	size_t from_size, p_from_size;
	char to[BUF_SIZE];
	char *p_to;
	size_t to_size, p_to_size;
	char enc[32];
	int score;

	fgets(from, BUF_SIZE, stdin);

	p_from = (const char *)from;
	p_to = to;
	to_size = p_to_size = BUF_SIZE;
	from_size = p_from_size = strlen(from);

	if (iconv_guess_convert("UTF-8", &p_from, &p_from_size, &p_to, &p_to_size, encodings, 0)==0) {
		printf("encoding not found\n");
		memcpy(to, from, p_from_size);
	}
	printf("%s\n", to);

	return 0;
}
