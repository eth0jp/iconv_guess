UNAME = ${shell uname}


all: example example_shared


example: iconv_guess.o example.o
	gcc -O iconv_guess.o example.o -liconv -o example


iconv_guess.o: iconv_guess.c
	gcc -O -c iconv_guess.c -o iconv_guess.o


example.o: example.c
	gcc -O -c example.c -o example.o


example_shared: libiconv_guess.so example.o
ifeq ($(UNAME), Linux)
	gcc -O example.o libiconv_guess.so -o example_shared
endif
ifeq ($(UNAME), Darwin)
	gcc -O example.o libiconv_guess.dylib -o example_shared
endif


libiconv_guess.so: iconv_guess.c
ifeq ($(UNAME), Linux)
	gcc -O --shared iconv_guess.c -liconv -o libiconv_guess.so
endif
ifeq ($(UNAME), Darwin)
	gcc -O -dynamiclib iconv_guess.c -liconv -o libiconv_guess.dylib
endif


install: example_shared
	cp iconv_guess.h /usr/local/include/
ifeq ($(UNAME), Linux)
	cp libiconv_guess.so /usr/local/lib/
endif
ifeq ($(UNAME), Darwin)
	cp libiconv_guess.dylib /usr/local/lib/
endif


uninstall:
	-rm /usr/loca/include/iconv_guess.h
	-rm /usr/local/lib/libiconv_guess.{so,dylib}


clean:
	-rm *.o
	-rm example
	-rm example_shared
	-rm libiconv_guess.{so,dylib}
