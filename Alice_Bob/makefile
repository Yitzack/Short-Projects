# Compiler
CC = g++
# Compiler flags
CFLAGS = -O3 -pthread -I./

# Source files
SRC = AES.cpp RSA.cpp SHA256.cpp GF256.cpp Hamming.cpp Server.cpp Client.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Static libraries
LIBS = libAES.a libRSA.a libSHA256.a libGF256.a libHamming.a

libraries: libAES.a libRSA.a libSHA256.a libGF256.a libHamming.a

all: libraries Server Client AES RSA SHA256 Hamming clean_objs

# Build libraries
libAES.a: AES.cpp GF256.cpp
	$(CC) $(CFLAGS) -c $^
	ar rcs $@ AES.o GF256.o
	rm -f AES.o GF256.o

libRSA.a: RSA.cpp
	$(CC) $(CFLAGS) -c $^
	ar rcs $@ RSA.o
	rm -f RSA.o

libSHA256.a: SHA256.cpp
	$(CC) $(CFLAGS) -c $^
	ar rcs $@ SHA256.o
	rm -f SHA256.o

libGF256.a: GF256.cpp
	$(CC) $(CFLAGS) -c $^
	ar rcs $@ GF256.o
	rm -f GF256.o

libHamming.a: Hamming.cpp
	$(CC) $(CFLAGS) -c $^
	ar rcs $@ Hamming.o
	rm -f Hamming.o

# Build server and client
AES: $(LIBS) AES_gen.cpp
	$(CC) $(CFLAGS) AES_gen.cpp -o $@ -L. -lAES -lGF256

RSA: $(LIBS) RSA_gen.cpp
	$(CC) $(CFLAGS) RSA_gen.cpp -o $@ -L. -lRSA

SHA256: $(LIBS) SHA256_gen.cpp
	$(CC) $(CFLAGS) SHA256_gen.cpp -o $@ -L. -lSHA256

Hamming: $(LIBS) Hamming_test.cpp
	$(CC) $(CFLAGS) Hamming_test.cpp -o $@ -L. -lHamming

Server: $(LIBS) Server.cpp
	$(CC) $(CFLAGS) Server.cpp -o $@ -L. -lAES -lRSA -lSHA256 -lGF256 -lHamming

Client: $(LIBS) Client.cpp
	$(CC) $(CFLAGS) Client.cpp -o $@ -L. -lAES -lRSA -lSHA256 -lGF256 -lHamming

clean_objs:
	rm -f $(OBJ)

clean:
	rm -f $(LIBS) Server Client AES RSA SHA256 Hamming
