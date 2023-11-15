# Pi by Monte Carlo

Calculate Pi using Monte Carlo techniques.

## Compile directions for g++

```g++ Pi.cpp -O3 -fopenmp -p```

## Details of operation

Using as many threads as your CPU will provide, it uses the Mersenne Twister RNG to sample the N-cube. It creats the LCM number of coordinates. It then groups them into the space for each sample and determines if each one is inside or outside the N-ball. Periodically, the master thread will run statistics on the results of each thread to get an average result and then it averages the averages to get an overall result. The average accuracy is volitile and subject to the whims of RNGsus. The precision is roughly monotonicly convergent with samples. The result is generally more accurate than it is precise. Both generally improve as a power rule with the number of samples, but accuracy is fickel as according to the whims of RNGsus.

## Ideas of improvement over simple single threaded dart thrower in 2D

* Multithreading
* RNG from <random> instead of from cstdlib
* Statistics of accuracy and precision
* Use more dimensions than just 2D
