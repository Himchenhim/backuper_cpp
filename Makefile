CC=g++
INCDIRS=-I.
OPT=-O0
CFLAGS=-Wall -pedantic -g $(INCDIRS) $(OPT)
COMP=-std=c++17
SSLFLAGS=-lssl -lcrypto
DBG=-g3 -fsanitize=address

CFILES=main.cpp CBackup.cpp CDataUnit.cpp CDirectory.cpp CFile.cpp hashes.cpp filesystem_work.cpp texts.cpp
OBJECTS=main.o CBackup.o CDataUnit.o CDirectory.o CFile.o hashes.o filesystem_work.o texts.o

BINARY=bin
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(COMP) $(SSLFLAGS)  $(DBG)

%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $^ $(COMP) $(DBG)
clean:
	rm -rf  $(OBJECTS)