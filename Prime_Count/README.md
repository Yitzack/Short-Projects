# Prime Counter

This program will count prime numbers up to a limit and determine which algorithm is faster. I'm going to use 3 algorithms of primality testing: 1) Miller-Rabin test, 2) Agrawal–Kayal–Saxena (AKS) test, and 3) Sieve of Eratosthenes. Obviously, this cannot go very high for some definition of high, but there is a change in how the AKS test works at 5690034 and I think I can go at least that far.

Miller-Rabin runs in 410 ms for N = 10,000,000. AKS runs in 40.68 hours. And the Sieve of Eratosthenes runs in 3.6 seconds. I'd like to come back to Sieve and try again as I haven't pulled out all stops to make it run at maxium speed.

## Compile directions for g++

g++ Count.cpp -O3 -fopenmp

