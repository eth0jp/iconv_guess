UNAME = ${shell uname}


all: example example_shared


example: iconv_guess.o example.o
ifeq ($(UNAME), Linux)
	gcc -O iconv_guess.o example.o -o example -W -Wall
endif
ifeq ($(UNAME), Darwin)
	gcc -O iconv_guess.o example.o -liconv -o example -W -Wall
endif


iconv_guess.o: iconv_guess.c
	gcc -O -c iconv_guess.c -o iconv_guess.o -W -Wall


example.o: example.c
	gcc -O -c example.c -o example.o -W -Wall


example_shared: libiconv_guess.so example.o
ifeq ($(UNAME), Linux)
	gcc -O example.o libiconv_guess.so -o example_shared -W -Wall
endif
ifeq ($(UNAME), Darwin)
	gcc -O example.o libiconv_guess.dylib -o example_shared -W -Wall
endif


libiconv_guess.so: iconv_guess.c
ifeq ($(UNAME), Linux)
	gcc -O --shared iconv_guess.c -o libiconv_guess.so -W -Wall
endif
ifeq ($(UNAME), Darwin)
	gcc -O -dynamiclib iconv_guess.c -liconv -o libiconv_guess.dylib -W -Wall
endif

test: example
	cat ./txt/utf8.txt | ./example
	cat ./txt/sjis.txt | ./example
	cat ./txt/eucjp.txt | ./example
	cat ./txt/ascii.txt | ./example

install: libiconv_guess.so
	cp iconv_guess.h /usr/local/include/
ifeq ($(UNAME), Linux)
	cp libiconv_guess.so /usr/local/lib/
endif
ifeq ($(UNAME), Darwin)
	cp libiconv_guess.dylib /usr/local/lib/
endif


uninstall:
	-rm /usr/local/include/iconv_guess.h
	-rm /usr/local/lib/libiconv_guess.so
	-rm /usr/local/lib/libiconv_guess.dylib


clean:
	-rm *.o
	-rm example
	-rm example_shared
	-rm libiconv_guess.so
	-rm libiconv_guess.dylib
