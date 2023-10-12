# Lorentz Monte Carlo

Integrate one or more Lorentz peaks over a finite interval using Monte Carlo methods in 1 or more dimensions.

## Compile directions for g++

g++ Simple_Monte_Carlo.cpp -O3 -fopenmp -p

g++ Importance_Monte_Carlo.cpp -O3 -fopenmp -p

## Details of operation

Simple_Monte_Carlo.cpp uses a uniform random varible to Monte Carlo integrate an ND Lorentz peak or Lorentz surface. Correct answers are included to check results.

Importance_Monte_Carlo.cpp uses a Cauchy distributed random varible to Monte Carlo integrate an ND Lorentz peak or Lorentz surface. Includes its own Cauchy distribution object as the one in \<random\> doesn't have PDF function. It should operate in the same way.

## Monte Carlo techniques to consider

* Stratified Sampling

  Sub-divide the region into sub-regions and add the results

* Importance Sampling

  Instead of uniformly sampling the region, use a distribution that is more similar to the function so that samples are collected from the important regions

* Control Variates

  Use a known result that is correlated to the desired result to reduce the variance of the result. That is calculate X*=X-Cov(X,Y)/Var(Y)(Y-E[Y]). X, Y, and Cov(X,Y) are calulated in the Monte Carlo. Var(Y) and E[Y] are known before the Monte Carlo

* Antithetic Variates

  Use to RNGs that are anti-correlated to calculate the result independently and then take the average. So sample with Uniform(a,b) and a+b-Uniform(a,b). Then, when finished, take the average of the two results.

* Quasi-Random Numbers

  Use a quasi-random sequence of numbers instead of a pseudo-random sequence. Examine Sobol sequences and Halton sequences.

* Adaptive Sampling

  Use adaptive sampling to allocate more points to important regions. Like stratified sampling except the regions are selected automatically.

* Markov Chain

  Use Markov Chain methods to do importance sampling. The samples will be correlated so care will be called for when using it.
