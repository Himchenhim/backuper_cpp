CC=g++
INCDIRS=-I.
OPT=-O0
CFLAGS=-Wall -pedantic -g $(INCDIRS) $(OPT)

CFILES=CBackup.cpp CDataUnit.cpp CDirectory.cpp CFile.cpp CLink.cpp
OBJECTS=CBackup.o CDataUnit.o CDirectory.o CFile.o CLink.o 

BINARY=bin
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $^
clean:
	rm -rf $(BINARY) $(OBJECTS)