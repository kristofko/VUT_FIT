CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

all: proj2



proj2: check_args.o proj2.o  proj2.h
	$(CC) $(CFLAGS) -o  $@ $^ -lpthread

proj2.o: proj2.c 
	$(CC)  $(CFLAGS) -fPIC -c $^ -o $@  -lpthread

check_args.o: check_args.c 
	$(CC) $(CFLAGS) -fPIC -c $^ -o $@  -lpthread

clean:
	rm -f proj2 *.o

zip:
	zip proj2.zip *.c *.h Makefile
