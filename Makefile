CC=g++
INCDIRS=-I.
OPT=-O0
CFLAGS=-Wall -pedantic -g $(INCDIRS) $(OPT)
COMP=-std=c++17
SSLFLAGS=-lssl -lcrypto

CFILES=main.cpp CBackup.cpp CDataUnit.cpp CDirectory.cpp CFile.cpp CLink.cpp hashes.cpp
OBJECTS=main.o CBackup.o CDataUnit.o CDirectory.o CFile.o CLink.o hashes.o

BINARY=bin
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(COMP) $(SSLFLAGS) -g3

%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $^ $(COMP) -g3
clean:
	rm -rf  $(OBJECTS)