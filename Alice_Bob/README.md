# Alice and Bob

Create a client program and server to send messages between to play with ideas of encryption, authentication, and communication between multiple clients and a server.  
RSA.h is the RSA library in object form. RSA_gen.cpp demonstrates the RSA algorithm.

Will probably copy and paste into a Boid Simulation.

## Compile directions for g++

First install the Boost library.  
g++ Client.cpp -o Client -pthread -O3  
g++ Server.cpp -o Server -pthread -O3

First install the Boost library.  
g++ RSA_gen.cpp -O3

## Features to be implemented

* AES Symmetric Encryption  
* Diffie-Hellman Key Exchange  
* SHA-512
