
PRJ=c206
#
PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm

all: $(PROGS)

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

clean:
	rm -f *.o  $(PROGS)
#

test:  $(PROGS)
	./$(PROGS) > test.out