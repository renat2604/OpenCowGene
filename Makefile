all: clean build

clean:
	rm -rf build/

build: build-math

build-math:
	mkdir -p build/ocgmath/include
	cp lib/ocgmath/ocgmath.h build/ocgmath/include/
	gcc -c lib/ocgmath/ocgmath.c --std=c99 -Wall -Wextra
	mv ocgmath.o build/ocgmath/
