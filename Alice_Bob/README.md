# Alice and Bob

Create a client program and server to send messages between to play with ideas of encryption, authentication, and communication between multiple clients and a server.  
RSA_gen.cpp generates a private and public key pair for RSA. This will become part of an RSA library for use by Client and Server. But for now, it is independent.

Will probably copy and paste into a Boid Simulation.

## Compile directions for g++

First install the Boost library.  
g++ Client.cpp -o Client -pthread  
g++ Server.cpp -o Server -pthread

g++ RSA_gen.cpp

## Features to be implemented

* RSA Public Key/Asymmetric Encryption  
* AES Symmetric Encryption  
* Diffie-Hellman Key Exchange  
* SHA-512
