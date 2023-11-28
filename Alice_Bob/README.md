# Alice and Bob

Create a client program and server to send messages between to play with ideas of encryption, authentication, and communication between multiple clients and a server.

RSA.h is the RSA library in object form. RSA_gen.cpp demonstrates the RSA algorithm.  
AES.h is the AES-256 library in object form. AES_gen.cpp demonstrates the AES-256 algorithm in ECB (Electronic Code Book) Mode. I'm not making more modes as the security application doesn't require it. GF256.h is a class for a Galois field of 256 elements. No combined operator, such as ```operator+=()``` or ```operator>=()```, were written, but most individual operators were written.  
SHA256.h is the SHA2-256 library in object form. SHA256_gen.cpp demonstrates the SHA256 algorithm.  
Hamming.h implements an extended Hamming(512,502) code. However, 502 is not a multiple of 8, but 496 is. I like working with whole bytes, so the 6 left over bits are parity bits of random, non-overlapping subsets of all message bits and appended at the end to increase the Hamming distance between valid codes. The effect is that in a random set of 512 bits, about 1.5% of them are valid codes.

Will probably copy and paste into a Boid Simulation and/or Conway's Game of Life after I get some Clients to talk with the Server simultanously.

## Compile directions for g++

You can use the included makefile to build everything. If you are missing the Boost library, it will let you know and build anything it can.

Build everything:  
```make all```

Build all libraries:  
```make libraries```

Build examples:  
Progtams included as examples: AES RSA SHA256 Hamming  
```make examples```

Build Server and Client:  
```make core```

Build specific library or program:  
Avalible libraries: libAES.a libRSA.a libSHA256.a libGF256.a libHamming.a  
Avalible programs: Server Client AES RSA SHA256 Hamming  
```make <library or program>```

Remove all builds:  
```make clean```
