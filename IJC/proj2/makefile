CC=gcc
CFLAGS=-std=c11 -Wall -pedantic
HEADERS=htab.h htab_t.h io.h
CFILES=wordcount.c io.c htab_hash_function.c  htab_move.c htab_for_each.c htab_erase.c htab_init.c htab_lookup_add.c htab_size.c htab_bucket_count.c htab_find.c htab_free.c htab_clear.c
OBJ=io.o htab_hash_function.o htab_for_each.o htab_move.o htab_erase.o htab_init.o htab_lookup_add.o htab_size.o htab_bucket_count.o htab_find.o htab_free.o htab_clear.o


all: tail wordcount wordcount-dynamic
	

wordcount: wordcount.o  $(HEADERS) libhtab.a
		
	$(CC) $(CFLAGS)  -o $@ -static $^ 

wordcount-dynamic: wordcount.o  $(HEADERS)	
	$(CC) $(CFLAGS) -o $@ $^ -L. -lhtab

libhtab.a: $(OBJ)
	@ar crs $@ $^ 

libhtab.so: $(OBJ)
	@$(CC) $(CFLAGS) -shared -o $@ $^


wordcount.o: wordcount.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c  wordcount.c -o  $@

io.o: io.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c io.c -o $@

htab_hash_function.o: htab_hash_function.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_hash_function.c -o  $@

htab_init.o: htab_init.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_init.c -o $@

htab_lookup_add.o: htab_lookup_add.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_lookup_add.c -o $@

htab_size.o: htab_size.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_size.c -o $@

htab_bucket_count.o: htab_bucket_count.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_bucket_count.c -o $@ 

htab_free.o: htab_free.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_free.c -o $@ 

htab_clear.o: htab_clear.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_clear.c -o $@  

htab_find.o: htab_find.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_find.c -o $@  

htab_for_each.o: htab_for_each.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_for_each.c -o $@  

htab_move.o: htab_move.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_move.c -o $@  

htab_erase.o: htab_erase.c $(HEADERS)
	@$(CC) $(CFLAGS) -fPIC -c htab_erase.c -o $@ 
 
run:
	./wordcount data.txt
	./wordcount-dynamic data.txt
tail: tail.c
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f libhtab.a libhtab.so tail all wordcount wordcount-dynamic init *.o 
zip:
	zip xsiska16 *.c *.cc *.h Makefile
