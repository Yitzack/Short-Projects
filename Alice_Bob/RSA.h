#include<iostream>
#include<cmath>
#include<random>
#include<limits>
#include<exception>
#include<string>
#include<boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost::multiprecision;

class LargeException : public exception
{
	public:
		enum class ErrorType	//Enumeration of error types
		{
			MessageTooLarge,
			CodeTooLarge
		};

		LargeException(ErrorType type) : errorType(type)	//Constructor of the LargeException
		{
			switch(type)	//Setting the error message based off the error type
			{
				case ErrorType::MessageTooLarge:
					errorMessage = "Message is greater than common key modulo";
					break;
				case ErrorType::CodeTooLarge:
					errorMessage = "Code is greater than common key modulo";
					break;
			}
		}

		virtual const char* what() const throw()	//Allow the error message to be retrived in a try/catch block
		{
			return(errorMessage.c_str());
		}
	private:
		ErrorType errorType;
		string errorMessage;
};

#ifndef RSA_HEADER
#define RSA_HEADER

class RSA
{
	public:
		RSA();					//Do nothing constructor
		RSA(cpp_int, cpp_int);			//Set the public key of the RSA object
		cpp_int Public_key_n();		//Output the public key n=pq
		cpp_int Public_key_e();		//Output the public key e such that (e*d)%lamda(n)=1
		cpp_int Encrypt(cpp_int);		//Encrypt with the message with the public key
		cpp_int Sign(cpp_int);			//Sign the message, that is encrypt with the message with the private key
		cpp_int Decrypt(cpp_int);		//Decrypt with the message with the private key
		void set_Public_key_n(cpp_int);	//Set the public key n=pq
		void set_Public_key_e(cpp_int);	//Set the public key e such that (e*d)%lambda(n)=1
		void initalize();			//Initalize the public and private key pairs
		void Print_Private_Key(ostream&);	//Print the private key, under no circumstance shall this function provide the private key in program. It shall only go out of program to file, stdout, or stderr. The user will have to release the private key themselves directly or load it back in themselves.
	private:
		cpp_int private_key_d;
		cpp_int key_n;
		cpp_int public_key_e;
		bool AKS_Prime_Test(cpp_int);		//AKS primaility test (not used in the current implemenation because it takes forever)
		bool prime_test(cpp_int);		//Probablistic prime test, uses the first 100 primes as witness numbers. Primes are particularlly good at the task.
		cpp_int Carmichael(cpp_int, cpp_int);	//Carmichael Totient Function of p*q (lcm(p-1, q-1)=(q-1)(p-1)/(gcd(p-1, q-1)))
		cpp_int Euclidean(cpp_int, cpp_int);	//Extended Euclidean algorithm to solve 1=(d*e)%lambda
		cpp_int GCD(cpp_int, cpp_int);	//Greatest common denominator
		cpp_int EulerTotient(cpp_int);	//Euler Totient Function
		cpp_int PowMod(cpp_int b, cpp_int n, cpp_int m);	//returns (b^n)%m
		cpp_int pow(cpp_int b, cpp_int n);	//return b^n
};
#endif

RSA::RSA()
{
	private_key_d = 0;
	key_n = 0;
	public_key_e = 65537;
}

RSA::RSA(cpp_int n, cpp_int e)
{
	key_n = n;
	public_key_e = e;
}

cpp_int RSA::Public_key_n()
{
	return(key_n);
}

cpp_int RSA::Public_key_e()
{
	return(public_key_e);
}

cpp_int RSA::Encrypt(cpp_int message)
{
	if(message > key_n)
	{
		throw(LargeException(LargeException::ErrorType::MessageTooLarge));
	}

	return(PowMod(message,public_key_e,key_n));
}

cpp_int RSA::Sign(cpp_int message)
{
	if(message > key_n)
	{
		throw(LargeException(LargeException::ErrorType::MessageTooLarge));
	}

	return(PowMod(message,private_key_d,key_n));
}

cpp_int RSA::Decrypt(cpp_int code)
{
	if(code > key_n)
	{
		throw(LargeException(LargeException::ErrorType::CodeTooLarge));
	}

	return(PowMod(code,private_key_d,key_n));
}

void RSA::set_Public_key_n(cpp_int n)
{
	key_n = n;
	return;
}

void RSA::set_Public_key_e(cpp_int e)
{
	public_key_e = e;
	return;
}

void RSA::Print_Private_Key(ostream& out)
{
	out << "Under no circumstance should you share this with anyone else. It is a private key and should remain that way." << endl;
	out << hex << private_key_d << endl;
	return;
}

void RSA::initalize()
{
	cpp_int p, q;		//p and q are large primes
	cpp_int lambda;	//Carmichael Totient of n

	mt19937 RNG(time(NULL));
	uniform_int_distribution<unsigned long long int> Prime(0,4294967295);

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

		key_n = p*q;
		lambda = Carmichael(p,q);
		private_key_d = Euclidean(public_key_e,lambda);
	}while((private_key_d*public_key_e)%lambda != 1);
}

cpp_int RSA::Euclidean(cpp_int a, cpp_int b)
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

bool RSA::prime_test(cpp_int n)
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
	for(int i = 0; i < 100; i++)	//Miller-Rabin test with the first 12 primes as bases which exceeds 2^64 by 14 orders of magnitude. I use 100 primes to get 4096-bits
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

cpp_int RSA::PowMod(cpp_int b, cpp_int n, cpp_int m)
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

cpp_int RSA::pow(cpp_int b, cpp_int n)
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

cpp_int RSA::Carmichael(cpp_int p, cpp_int q)
{
	return((q-1)*(p-1)/(GCD(p-1, q-1)));
}

cpp_int RSA::GCD(cpp_int u, cpp_int v)
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

cpp_int RSA::EulerTotient(cpp_int a)
{
	cpp_int answer = 0;
	for(cpp_int i = 1; i <= a; i++)
		if(GCD(i, a)==1) answer++;
	return(answer);
}

bool RSA::AKS_Prime_Test(cpp_int n)
{
	//Test perfect power
	for(int b = 2; b <= log((long double)(n))/log(2.); b++)
	{
		double a = std::pow((long double)(n), 1./double(b));
		if(floor(a) == ceil(a))
			return(false);
	}

	//Find the smallest r such that O_r(n)>(log_2(n))^2
	int maxk = std::pow(log((long double)(n))/log(2.), 2);
	int maxr = max((long double)(3.), ceil(std::pow(log((long double)(n))/log(2.), 5)));
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
