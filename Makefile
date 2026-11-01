all: clean build

clean:
	rm -rf build/

build: build-math build-v2
	gcc -o build/ocg-main main.c \
		build/ocgv2/ocgv2.o build/ocgmath/ocgmath.o \
		-I"build/ocgmath/include" -I"build/ocgv2/include" \
		-lm \
		--std=c99 -Wall -Wextra

build-math:
	mkdir -p build/ocgmath/include
	cp lib/ocgmath/*.h build/ocgmath/include/
	gcc -c lib/ocgmath/ocgmath.c --std=c99 -Wall -Wextra
	mv ocgmath.o build/ocgmath/

build-v2:
	mkdir -p build/ocgv2/include
	cp lib/ocgv2/*.h build/ocgv2/include/
	gcc -c lib/ocgv2/ocgv2.c \
		-I"build/ocgmath/include" \
		--std=c99 -Wall -Wextra 
	mv ocgv2.o build/ocgv2/

run: build
	exec build/ocg-main
