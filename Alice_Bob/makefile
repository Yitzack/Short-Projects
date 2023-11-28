# Compiler
CC = g++
# Compiler flags
CFLAGS = -O3 -pthread -std=c++17 -I./

# Source files
SRC = AES.cpp RSA.cpp SHA256.cpp GF256.cpp Hamming.cpp Server.cpp Client.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Static libraries
LIBS = libAES.a libRSA.a libSHA256.a libGF256.a libHamming.a

all: Server Client

# Compile objects
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Build libraries
libAES.a: AES.o GF256.o
	ar rcs $@ $^

libRSA.a: RSA.o
	ar rcs $@ $^

libSHA256.a: SHA256.o
	ar rcs $@ $^

libGF256.a: GF256.o
	ar rcs $@ $^

libHamming.a: Hamming.o
	ar rcs $@ $^

# Build server and client
Server: Server.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@
	rm *.o

Client: Client.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@
	rm *.o

clean:
	rm -f $(OBJ) $(LIBS) Server Client
