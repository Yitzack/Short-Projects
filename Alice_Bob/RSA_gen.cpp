#include<iostream>
#include<cmath>
#include<random>
#include<limits>
#include<chrono>
#include<boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost::multiprecision;

bool AKS_Prime_Test(cpp_int);	//AKS primaility test
bool prime_test(cpp_int);		//Probablistic prime test, uses the first 100 primes as witness numbers. Primes are particularlly good at the task.
cpp_int Carmichael(cpp_int, cpp_int);	//Carmichael Totient Function of p*q (lcm(p-1, q-1)=(q-1)(p-1)/(gcd(p-1, q-1)))
cpp_int Euclidean(cpp_int, cpp_int);	//Extended Euclidean algorithm to solve 1=(d*e)%lambda
cpp_int GCD(cpp_int, cpp_int);
cpp_int EulerTotient(cpp_int);	//Euler Totient Function
cpp_int PowMod(cpp_int b, cpp_int n, cpp_int m);	//returns (b^n)%m

int main()
{
	cpp_int p, q;	//p and q are large primes
	cpp_int n;	//n = pq, public key
	cpp_int lambda;	//Carmichael Totient of n
	cpp_int e = 65537;	//Common choice for e as it needs to be coprime with lambda and primes are coprime with everything. It should also be less than lambda. Part of public key
	cpp_int d;		//private key, solves 1=(d*e)%lambda

	mt19937 RNG(time(NULL));
	uniform_int_distribution<unsigned long long int> Prime(0,4294967295);
	uniform_int_distribution<unsigned long long int> message(pow(2,56)-1, pow(2,64)-1);

	do
	{
		q = p = 0;
		for(int i = 0; i < 63; i++)
		{
			p += Prime(RNG);
			p <<= 32;
		}
		p += Prime(RNG);
		if(!(p&1))
			p++;
		while(!prime_test(p))
		{
			p = p ^ (Prime(RNG) << 1);
		}

		for(int i = 0; i < 63; i++)
		{
			q += Prime(RNG);
			q <<= 32;
		}
		q += Prime(RNG);
		if(!(q&1))
			q++;
		while(!prime_test(q))
		{
			q = q ^ (Prime(RNG) << 1);
		}

		n = p*q;
		lambda = Carmichael(p,q);
		d = Euclidean(e,lambda);
	}while((d*e)%lambda != 1);

	cout << hex << "p: " << p << "\nq: " << q << "\nlambda: " << lambda << "\nn: " << n << endl;
	cout << "Public key:  " << n << "," << e << endl;
	cout << "Private key: " << n << "," << d << endl;
	
	cpp_int Message;

	for(int i = 0; i < 126; i++)
	{
		Message += Prime(RNG);
		Message <<= 32;
	}
	Message += Prime(RNG);

	cpp_int Cypher = PowMod(Message,d,n);
	cpp_int Decrypt = PowMod(Cypher,e,n);

	cout << "Message:  " << Message << endl;
	cout << "Cypher:   " << Cypher << endl;
	cout << "Message': " << Decrypt << endl;
	if(Message == Decrypt)
		cout << "Success" << endl;
	else
		cout << "Error" << endl;

	return(0);
}

cpp_int Euclidean(cpp_int a, cpp_int b)
{
	pair<cpp_int,cpp_int> r(a,b);
	pair<cpp_int,cpp_int> s(1,0);
	pair<cpp_int,cpp_int> t(0,1);
	cpp_int quotient;

	while(r.second != 0)
	{
		quotient = r.first/r.second;
		r = pair<cpp_int,cpp_int>(r.second, r.first-quotient*r.second);
		s = pair<cpp_int,cpp_int>(s.second, s.first-quotient*s.second);
		t = pair<cpp_int,cpp_int>(t.second, t.first-quotient*t.second);
	}

	return(s.first);
}

bool prime_test(cpp_int n)
{
	int Primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541}; //100 primes

	for(int i = 0; i < 100; i++)	//Simple prime test with the first 100 primes
		if(n%Primes[i] == 0)
			return(false);

	cpp_int d = n-1;
	cpp_int test1, test2;
	int s = 0;

	while(d%2 == 0)
	{
		d >>= 1;
		s++;
	}
	for(int i = 0; i < 100; i++)	//Miller-Rabin test with the first 12 primes as bases which exceeds 2^64 by 14 orders of magnitude
	{
		cpp_int a = Primes[i];
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

cpp_int PowMod(cpp_int b, cpp_int n, cpp_int m)
{
	if(m == 1)
		return(0);

	cpp_int answer = 1;
	cpp_int bint = b%m;

	while(n > 0)
	{
		if(n%2 == 1)
			answer = (answer*bint)%m;
		n >>= 1;
		bint = (bint*bint)%m;
	}

	return(answer);
}

cpp_int pow(cpp_int b, cpp_int n)
{
	cpp_int answer = 1;
	cpp_int bint = b;

	while(n > 0)
	{
		if(n%2 == 1)
			answer *= bint;
		n >>= 1;
		bint *= bint;
	}

	return(answer);
}

cpp_int Carmichael(cpp_int p, cpp_int q)
{
	return((q-1)*(p-1)/(GCD(p-1, q-1)));
}

cpp_int GCD(cpp_int u, cpp_int v)
{
	cpp_int answer = 1;
	if(u == 0)
		return(v);
	else if(v == 0)
		return(u);

	cpp_int temp = u|v;
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
			cpp_int temp = u;
			u = v;
			v = temp;
		}
		u -= v;
		while(u % 2 == 0)
			u >>= 1;
	}

	return(answer * u);
}

cpp_int EulerTotient(cpp_int a)
{
	cpp_int answer = 0;
	for(cpp_int i = 1; i <= a; i++)
		if(GCD(i, a)==1) answer++;
	return(answer);
}

bool AKS_Prime_Test(cpp_int n)
{
	//Test perfect power
	for(int b = 2; b <= log((long double)(n))/log(2.); b++)
	{
		double a = pow((long double)(n), 1./double(b));
		if(floor(a) == ceil(a))
			return(false);
	}

	//Find the smallest r such that O_r(n)>(log_2(n))^2
	int maxk = pow(log((long double)(n))/log(2.), 2);
	int maxr = max((long double)(3.), ceil(pow(log((long double)(n))/log(2.), 5)));
	int r;
	bool nextR = true;
	for(r = 2; nextR && r < maxr; r++)
	{
		nextR = false;
		for(int k = 1; !nextR && k <= maxk; k++)
		{
			cpp_int test = pow(n, k);
			test = test%r;
			nextR = (test == 1 || test == 0);
		}
	}
	r--;

	//GCD test
	for(int a = r; a > 1; a--)
	{
		cpp_int gcd = GCD(a, n);
		if(1 < gcd && gcd < n)
			return(false);
	}

	//if n <= r then prime. If n is larger than 5690034, then this test is always false
	if(n <= r)
		return(true);

	//The real prime test
	unsigned long long int max = ceil(log((long double)(n))/log(2.)*(long double)(sqrt(EulerTotient(r))));
	for(cpp_int a = 1; a < max; a++)
	{
		cpp_int test = pow(a, n)-a;
		if(test%n != 0)
			return(false);
	}

	return(true);
}
