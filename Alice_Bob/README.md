# Alice and Bob

Create a client program and server to send messages between to play with ideas of encryption, authentication, and communication between multiple clients and a server.  
RSA.h is the RSA library in object form. RSA_gen.cpp demonstrates the RSA algorithm.  
AES.h is the AES-256 library in object form. AES_gen.cpp demonstrates the AES-256 algorithm in ECB (Electronic Code Book) Mode. I'm not making more modes as the security application doesn't require it. GF256.h is a class for a Galois field of 256 elements. No combined operator, such as ```operator+=()``` or ```operator>=()```, were written, but most individual operators were written.  
SHA256.h is the SHA2-256 library in object form. SHA256_gen.cpp demonstrates the SHA256 algorithm.

Will probably copy and paste into a Boid Simulation and/or Conway's Game of Life.

## Compile directions for g++

First install the Boost library.  
```g++ Client.cpp -o Client -pthread -O3```  
```g++ Server.cpp -o Server -pthread -O3```

First install the Boost library.  
```g++ RSA_gen.cpp -O3```

No dependency on the Boost library.  
```g++ AES_gen.cpp -O3```

No dependency on the Boost library.  
```g++ SHA256_gen.cpp -O3```

First install the Boost library.  
```g++ Hamming_test.cpp -O3```

## Future features

* Extended Hamming code, Hammaing(511,502)+total parity
