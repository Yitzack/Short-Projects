#include<iostream>
#include<cmath>
#include<random>
#include<limits>
#include<chrono>
using namespace std;

bool AKS_Prime_Test(unsigned long int);	//AKS primaility test
bool prime_test(unsigned long int);		//Probablistic prime test, using 12 witness numbers turns it from probable to deterministic
unsigned long int Carmichael(unsigned long int, unsigned long int);	//Carmichael Totient Function of p*q (lcm(p-1, q-1)=(q-1)(p-1)/(gcd(p-1, q-1)))
unsigned long int Euclidean(unsigned long int, unsigned int);	//Extended Euclidean algorithm to solve 1=(d*e)%lambda
unsigned long int GCD(unsigned long int, unsigned long int);
unsigned long int EulerTotient(unsigned long int);	//Euler Totient Function
unsigned long int PowMod(unsigned long int b, unsigned long int n, unsigned long int q);	//returns (b^n)%q

int main()
{
	unsigned long int p, q;	//p and q are large primes
	unsigned long long int n;	//n = pq, public key
	unsigned long int lambda;	//Carmichael Totient of n
	unsigned int e = 65537;	//Common choice for e as it needs to be coprime with lambda and primes are coprime with everything. It should also be less than lambda. Part of public key
	unsigned long int d;		//private key, solves 1=(d*e)%lambda

	mt19937 RNG(time(NULL));
	uniform_int_distribution<unsigned long int> Uniform(numeric_limits<int>::max(), numeric_limits<unsigned long int>::max());

	/*cout << numeric_limits<int>::max() << " " << numeric_limits<unsigned long int>::max() << endl;*/

p = 11068403342312893979;//Uniform(RNG);
auto start = chrono::high_resolution_clock::now();
if(prime_test(p))
	cout << p << " is prime" << endl;
else
	cout << p << " is composite" << endl;
auto end = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
cout << "Time taken by the function: " << duration << " nanoseconds" << endl;

	/*do
	{
		p = Uniform(RNG);
	}while(!prime_test(p));
	do
	{
		q = Uniform(RNG);
	}while(!prime_test(q));
	n = p*q;

	cout << "p: " << p << " q: " << q << " n: " << n << endl;*/

	return(0);
}

bool prime_test(unsigned long int n)
{
	int Primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541}; //100 primes

	for(int i = 0; i < 100; i++)	//Simple prime test with the first 100 primes
		if(n%Primes[i] == 0)
			return(false);

	unsigned long int d = n-1;
	int test;
	int s = 0;
	while(d%2 == 0)
	{
		d >>= 1;
		s++;
	}
	for(int i = 0; i < 12; i++)	//Miller-Rabin test with the first 12 primes as bases which exceeds 2^64 by 14 orders of magnitude
	{
		unsigned long int a = Primes[i];
		test = PowMod(a,d,n);
cout << a << "^" << d << '%' << n << "=" << test << endl;
		if(!(test == 1 || test == n-1))
			return(false);
	}
	return(true);
}

unsigned long int PowMod(unsigned long int b, unsigned long int n, unsigned long int m)
{
	if(m == 1)
		return(0);

	int answer = 1;
	b %= m;

	while(n > 0)
	{
		if(n%2 == 1)
			answer = (answer*b)%m;
		n >>= 1;
		b = (b*b)%m;
	}

	return(answer);
}

unsigned long int Carmichael(unsigned long int p, unsigned long int q)
{
	return((q-1)*(p-1)/(GCD(p-1, q-1)));
}

unsigned long int GCD(unsigned long int u, unsigned long int v)
{
	unsigned long int answer = 1;
	if(u == 0)
		return(v);
	else if(v == 0)
		return(u);

	unsigned long int temp = u|v;
	while(temp % 2 == 0)
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
		while(u % 2 == 0)
			u >>= 1;
	}

	return(answer * u);
}

unsigned long int EulerTotient(unsigned long int a)
{
	unsigned long int answer = 0;
	for(unsigned long int i = 1; i <= a; i++)
		if(GCD(i, a)==1) answer++;
	return(answer);
}

bool AKS_Prime_Test(unsigned long int n)
{
	//Test perfect power
	for(int b = 2; b <= log(n)/log(2.); b++)
	{
		double a = pow(n, 1./double(b));
		if(floor(a) == ceil(a))
			return(false);
	}

	//Find the smallest r such that O_r(n)>(log_2(n))^2
	int maxk = pow(log(n)/log(2.), 2);
	int maxr = max(3., ceil(pow(log(n)/log(2.), 5)));
	int r;
	bool nextR = true;
	for(r = 2; nextR && r < maxr; r++)
	{
		nextR = false;
		for(int k = 1; !nextR && k <= maxk; k++)
		{
			unsigned long long int test = pow(n, k);
			test = test%r;
			nextR = (test == 1 || test == 0);
		}
	}
	r--;

	//GCD test
	for(int a = r; a > 1; a--)
	{
		int gcd = GCD(a, n);
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
		unsigned long long int test = pow(a, n)-a;
		if(test%n != 0)
			return(false);
	}

	return(true);
}
