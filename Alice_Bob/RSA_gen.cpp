#include<iostream>
#include<cmath>
#include<random>
#include<limits>
#include<chrono>
#include<boost/multiprecision/gmp.hpp>
using namespace std;

bool AKS_Prime_Test(unsigned long int);	//AKS primaility test
bool prime_test(unsigned long int);		//Probablistic prime test, using 12 witness numbers turns it from probable to deterministic for all numbers less than 2^65-1
unsigned long int Carmichael(unsigned long int, unsigned long int);	//Carmichael Totient Function of p*q (lcm(p-1, q-1)=(q-1)(p-1)/(gcd(p-1, q-1)))
unsigned long int Euclidean(unsigned long int, unsigned long int);	//Extended Euclidean algorithm to solve 1=(d*e)%lambda
unsigned long int GCD(unsigned long int, unsigned long int);
unsigned long int EulerTotient(unsigned long int);	//Euler Totient Function
unsigned long long int PowMod(unsigned long long int b, unsigned long long int n, unsigned long long int m);	//returns (b^n)%m

int main()
{
	unsigned long long int p, q;	//p and q are large primes
	unsigned long long int n;	//n = pq, public key
	unsigned long long int lambda;	//Carmichael Totient of n
	unsigned long long int e = 65537;	//Common choice for e as it needs to be coprime with lambda and primes are coprime with everything. It should also be less than lambda. Part of public key
	unsigned long long int d;		//private key, solves 1=(d*e)%lambda

	mt19937 RNG(time(NULL));
	uniform_int_distribution<unsigned int> Prime(255,16777215);
	uniform_int_distribution<unsigned int> message(0, 16777215);

	do
	{
		do
		{
			p = Prime(RNG);
		}while(!prime_test(p));
		do
		{
			q = Prime(RNG);
		}while(!prime_test(q));
		n = p*q;
		lambda = Carmichael(p,q);
		d = Euclidean(e,lambda);
	}while(((d*e)%lambda != 1) || (n < 2147483647 || n > 4294967295));

	//cout << "p: " << p << " q: " << q << " lambda: " << lambda << " n: " << n << endl;
	//cout << hex;
	cout << "Public key:  " << n << "," << e << endl;
	cout << "Private key: " << n << "," << d << endl;
	
	unsigned long long int Message = message(RNG);
	unsigned long long int Cypher = PowMod(Message,d,n);
	unsigned long long int Decrypt = PowMod(Cypher,e,n);

	cout << "Message:  " << Message << endl;
	cout << "Cypher:   " << Cypher << endl;
	cout << "Message': " << Decrypt << endl;

	return(0);
}

unsigned long int Euclidean(unsigned long int a, unsigned long int b)
{
	pair<unsigned long int,unsigned long int> r(a,b);
	pair<unsigned long int,unsigned long int> s(1,0);
	pair<unsigned long int,unsigned long int> t(0,1);
	unsigned long int quotient;

	while(r.second != 0)
	{
		quotient = r.first/r.second;
		r = pair<unsigned long int,unsigned long int>(r.second, r.first-quotient*r.second);
		s = pair<unsigned long int,unsigned long int>(s.second, s.first-quotient*s.second);
		t = pair<unsigned long int,unsigned long int>(t.second, t.first-quotient*t.second);
	}

	return(s.first);
}

bool prime_test(unsigned long int n)
{
	int Primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541}; //100 primes

	for(int i = 0; i < 100; i++)	//Simple prime test with the first 100 primes
		if(n%Primes[i] == 0)
			return(false);

	unsigned long long int d = n-1;
	unsigned long long int test1, test2;
	int s = 0;

	while(d%2 == 0)
	{
		d >>= 1;
		s++;
	}
	for(int i = 0; i < 12; i++)	//Miller-Rabin test with the first 12 primes as bases which exceeds 2^64 by 14 orders of magnitude
	{
		unsigned long long int a = Primes[i];
		test1 = PowMod(a,d,n);
		for(int j = 0; j < s; j++)
		{
			test2 = PowMod(test1,2,n);
			if(test2 == 1 && test1 != 1 && test1 != n-1)
				return(false);
			test1 = test2;
		}
		if(test2 != 1)
			return(false);
	}
	return(true);
}

unsigned long long int PowMod(unsigned long long int b, unsigned long long int n, unsigned long long int m)
{
	if(m == 1)
		return(0);

	unsigned long long int answer = 1;
	unsigned long long int bint = b%m;

	while(n > 0)
	{
		if(n%2 == 1)
			answer = (answer*bint)%m;
		n >>= 1;
		bint = (bint*bint)%m;
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
