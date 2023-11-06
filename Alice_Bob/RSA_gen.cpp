#include<iostream>
#include<cmath>
using namespace std;

bool prime_test(unsigned long int);	//AKS primaility test
unsigned long int Carmichael(unsigned long int, unsigned long int);	//Carmichael Totient Function of p*q (lcm(p-1,q-1)=(q-1)(p-1)/(gcd(p-1,q-1)))
unsigned long int Euclidean(unsigned long int, unsigned int);	//Extended Euclidean algorithm to solve 1=(d*e)%lambda
unsigned long int GCD(unsigned long int, unsigned long int);
unsigned long int EulerTotient(unsigned long int);	//Euler Totient Function

int main()
{
	unsigned long int p, q;	//p and q are large primes
	unsigned long long int n;	//n = pq, public key
	unsigned long int lambda;	//Carmichael Totient of n
	unsigned int e = 65537;	//Common choice for e as it needs to be coprime with lambda and primes are coprime with everything. It should also be less than lambda. Part of public key
	unsigned long int d;		//private key, solves 1=(d*e)%lambda

	return(0);
}

unsigned long int Carmichael(unsigned long int p, unsigned long int q)
{
	return((q-1)*(p-1)/(GCD(p-1,q-1)));
}

unsigned long int GCD(unsigned long int u, unsigned long int v)
{
	unsigned long int answer = 1;
	if(u == 0)
		return(v);
	else if(v == 0)
		return(u);

	unsigned long int temp = u|v;
	while(!(temp|1))
	{
		answer <<= 1;
		u >>= 1;
		v >>= 1;
		temp >>= 1;
	}

	while(u != v)
	{
		if(u < v)
		{
			unsigned long int temp = u;
			u = v;
			v = temp;
		}
		u -= v;
		while(!(u|1))
			u >>= 1;
	}

	return(answer * u);
}

unsigned long int EulerTotient(unsigned long int a)
{
	unsigned long int answer = 0;
	for(unsigned long int i = 1; i <= a; i++)
		if(GCD(i,a)==1) answer++;
	return(answer);
}

bool prime_test(unsigned long int n)
{
	//Test perfect power
	for(int b = 2; b <= log(n)/log(2.); b++)
	{
		double a = pow(n,1./double(b));
		if(floor(a) == ceil(a))
			return(false);
	}

	//Find the smallest r such that O_r(n)>(log_2(n))^2
	int maxk = pow(log(n)/log(2.),2);
	int maxr = max(3.,ceil(pow(log(n)/log(2.),5)));
	int r;
	bool nextR = true;
	for(r = 2; nextR && r < maxr; r++)
	{
		nextR = false;
		for(int k = 1; !nextR && k <= maxk; k++)
		{
			unsigned long long int test = pow(n,k);
			test = test%r;
			nextR = (test == 1 || test == 0);
		}
	}
	r--;

	//GCD test
	for(int a = r; a > 1; a--)
	{
		int gcd = GCD(a,n);
		if(1 < gcd && gcd < n)
			return(false);
	}

	//if n <= r then prime. If n is larger than 5690034, then this test is always false
	if(n <= r)
		return(true);

	//The real prime test
	long int max = floor(log(n)/log(2.)*sqrt(EulerTotient(r)));
	for(long int a = 1; a < max; a++)
	{
		unsigned long long int test = pow(a,n)-a;
		if(test%n != 0)
			return(false);
	}

	return(true);
}
