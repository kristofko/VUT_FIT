CC=g++
CFLAGS=-lpcap
HEADERS=arg_parser.h flow.h exporter.h
CFILES=arg_parser.cpp exporter.cpp
OBJ=arg_parser.o exporter.o

flow: flow.cpp $(OBJ) $(HEADERS)
	$(CC)  -o $@ $^ $(CFLAGS)

arg_parser.o: arg_parser.cpp $(HEADERS)
	$(CC)  -fPIC -c arg_parser.cpp -o  $@ $(CFLAGS)

exporter.o: exporter.cpp $(HEADERS)
	$(CC)  -fPIC -c exporter.cpp -o  $@ $(CFLAGS)

clean: 
	rm -f *.o flow