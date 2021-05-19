CC=gcc
CFLAGS=-O2 -g -std=c11 -pedantic -Wall -Wextra
LFLAGS=-lm

all: primes steg-decode

run: primes
	./primes
	./primes-i

primes: primes.c eratosthenes.c error.c bitset.h
		$(CC) $(CFLAGS)  -o $@ $^ $(LDFLAGS)
		$(CC) -DUSE_INLINE bitset.c  $(CFLAGS)  $^ -o primes-i $(LDFLAGS)
 
steg-decode: steg-decode.c eratosthenes.c ppm.c error.c bitset.h
		$(CC) $(CFLAGS) -o $@ $^

clean: 
	rm -f steg-decode primes primes-i xsiska16.zip

zip: 
	zip xsiska16 *.c *.h makefile
