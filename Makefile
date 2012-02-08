UNAME = ${shell uname}


all: build


build:
	@echo "# build for $(UNAME)"
	mkdir build
	gcc iconv_guess.c test.c -liconv -o build/test_c
ifeq ($(UNAME), Linux)
	gcc --shared iconv_guess.c -liconv -o libiconv_guess.so
	mv libiconv_guess.so build/
	gcc test_shared.c build/libiconv_guess.so -o build/test_shared
endif
ifeq ($(UNAME), Darwin)
	gcc -dynamiclib iconv_guess.c -liconv -o libiconv_guess.dylib
	mv libiconv_guess.dylib build/
	gcc test_shared.c build/libiconv_guess.dylib -o build/test_shared
endif


test: build
	cat ./txt/sjis.txt | ./build/test_c
	cd build; cat ../txt/sjis.txt | ./test_shared


install: build
	cp iconv_guess.h /usr/local/include/
ifeq ($(UNAME), Linux)
	cp build/libiconv_guess.so /usr/local/lib/
endif
ifeq ($(UNAME), Darwin)
	cp build/libiconv_guess.dylib /usr/local/lib/
endif


uninstall:
	rm -rf /usr/loca/include/iconv_guess.h
	rm -rf /usr/local/lib/libiconv_guess.{so,dylib}


clean:
	rm -rf build
