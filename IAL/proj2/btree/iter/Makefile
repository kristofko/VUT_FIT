CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm
FILES=btree.c ../btree.c stack.c ../test_util.c ../test.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -g -o $@ $(FILES)

clean:
	rm -f test
