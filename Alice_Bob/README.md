# Alice and Bob

Create a client program and server to send messages between to play with ideas of encryption, authentication, and communication between multiple clients and a server.  
RSA.h is the RSA library in object form. RSA_gen.cpp demonstrates the RSA algorithm.  
AES.h is the AES library in object form. AES_gen.cpp demonstrates the AES algorithm in ECB (Electronic Code Book) Mode. I'm not making more modes as the security application doesn't require it. GF256.h is an class for a Galois field of 256 elements. No combined operator, such as ```operator+=()``` or ```operator>=()```, were written, but most individual operators were written.

Will probably copy and paste into a Boid Simulation.

## Compile directions for g++

First install the Boost library.  
```g++ Client.cpp -o Client -pthread -O3```  
```g++ Server.cpp -o Server -pthread -O3```

First install the Boost library.  
```g++ RSA_gen.cpp -O3```

No dependency on the Boost library.
```g++ AES_gen.cpp -O3```

## Features to be implemented

* SHA-512
