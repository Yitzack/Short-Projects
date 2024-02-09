#include<iostream>
#include<cstdint>
#include<cmath>
#include<chrono>
#include"omp.h"
using namespace std;

bool MR_Prime_Test(uint64_t);
uint64_t PowMod(uint64_t, uint64_t, uint64_t);
uint64_t GCD(uint64_t, uint64_t);
uint64_t EulerTotient(uint64_t);
bool AKS_Prime_Test(uint64_t);
uint64_t MR_Prime_Count(uint64_t);
uint64_t AKS_Prime_Count(uint64_t);
uint64_t E_Prime_Count(uint64_t);

int main()
{
	int count;
	auto start_time = chrono::high_resolution_clock::now();
	count = MR_Prime_Count(10000000);
	auto end_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
	cout << "There are 664579 primes less than 10000000.\n";
	cout << "Time taken to find " << count << " primes less than 10000000 using Miller-Rabin: " << duration.count() << " ms" << endl;

	start_time = chrono::high_resolution_clock::now();
	count = E_Prime_Count(10000000);
	end_time = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
	cout << "Time taken to find " << count << " primes less than 10000000 using Sieve of Eratosthenes: " << duration.count() << " ms" << endl;

	/*start_time = chrono::high_resolution_clock::now();
	count = AKS_Prime_Count(10000000);
	end_time = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
	cout << "Time taken to find " << count << " primes less than 10000000 using Agrawal–Kayal–Saxena: " << duration.count() << " ms" << endl;*/

	return(0);
}

uint64_t MR_Prime_Count(uint64_t N)
{
	uint64_t Answer = 1;

	#pragma omp parallel for
	for(uint64_t i = 3; i <= N; i += 2)
	{
		if(MR_Prime_Test(i))
		{
			#pragma omp atomic
			Answer++;
		}
	}

	return(Answer);
}

uint64_t AKS_Prime_Count(uint64_t N)
{
	uint64_t Answer = 1;
	auto currentTimePoint = chrono::system_clock::now();
	time_t currentTime;
	tm* localTime;
	char timeString[100];  // Buffer for storing the formatted time string

	#pragma omp parallel for schedule(static, 1)
	for(uint64_t i = 3; i <= N; i += 2)
	{
		if(i % 1000 == 3)
		{
			currentTimePoint = chrono::system_clock::now();
			currentTime = chrono::system_clock::to_time_t(currentTimePoint);
			localTime = localtime(&currentTime);
			strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);
			cout << "Thread " << omp_get_thread_num() << " working on " << i << " at " << timeString << "." << endl;
		}

		if(AKS_Prime_Test(i))
		{
			#pragma omp atomic
			Answer++;
		}
	}

	return(Answer);
}

uint64_t E_Prime_Count(uint64_t N)
{
	bool* TestArray = new bool[N];
	uint64_t Answer[24], result = 0;
	TestArray[1-1] = false;

	for(uint64_t i = 2; i < N; i++)
		TestArray[i-1] = true;

	for(uint64_t i = 2; i <= sqrt(N); i++)
	{
		if(TestArray[i-1])
		{
			uint64_t count = i;

			#pragma omp parallel for
			for(uint64_t j = 2*i; j < N; j += count)
				TestArray[j-1] = false;
		}
	}

	#pragma omp parallel
	{
		Answer[omp_get_thread_num()] = 0;
		#pragma omp for
		for(uint64_t i = 0; i < N; i++)
		{
			if(TestArray[i])
				Answer[omp_get_thread_num()]++;
		}
	}

	for(uint64_t partial: Answer)
		result += partial;

	return(result);
}

bool MR_Prime_Test(uint64_t n)
{
	int Primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541}; //100 primes

	for(int i = 0; i < 100; i++)	//Simple prime test with the first 100 primes
	{
		if(n == Primes[i])	//Is it one of the known primes?
			return(true);

		if(n%Primes[i] == 0)	//Is it a factor of a known prime?
			return(false);
	}

	uint64_t d = n-1;
	uint64_t test1, test2;
	int s = 0;

	while(d%2 == 0)
	{
		d >>= 1;
		s++;
	}
	for(int i = 0; i < 12; i++)	//Miller-Rabin test with the first 12 primes as bases which exceeds 2^64 by 14 orders of magnitude. I use 100 primes to get 4096-bits
	{
		uint64_t a = Primes[i];
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

uint64_t PowMod(uint64_t b, uint64_t n, uint64_t m)
{
	if(m == 1)
		return(0);

	uint64_t answer = 1;
	b = b%m;

	while(n > 0)
	{
		if(n%2 == 1)
			answer = (answer*b)%m;
		n >>= 1;
		b = (b*b)%m;
	}

	return(answer);
}

uint64_t EulerTotient(uint64_t a)
{
	uint64_t answer = 0;
	for(uint64_t i = 1; i <= a; i++)
		if(GCD(i, a)==1) answer++;
	return(answer);
}

uint64_t GCD(uint64_t u, uint64_t v)
{
	uint64_t answer = 1;
	if(u == 0)
		return(v);
	else if(v == 0)
		return(u);

	uint64_t temp = u|v;
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
			uint64_t temp = u;
			u = v;
			v = temp;
		}
		u -= v;
		while(u % 2 == 0)
			u >>= 1;
	}

	return(answer * u);
}

bool AKS_Prime_Test(uint64_t n)
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
			uint64_t test = pow(n, k);
			test = test%r;
			nextR = (test == 1 || test == 0);
		}
	}
	r--;

	//GCD test
	for(int a = r; a > 1; a--)
	{
		uint64_t gcd = GCD(a, n);
		if(1 < gcd && gcd < n)
			return(false);
	}

	//if n <= r then prime. If n is larger than 5690034, then this test is always false
	if(n <= r)
		return(true);

	//The real prime test
	uint64_t max = ceil(log((long double)(n))/log(2.)*(long double)(sqrt(EulerTotient(r))));
	for(uint64_t a = 1; a < max; a++)
	{
		uint64_t test = pow(a, n)-a;
		if(test%n != 0)
			return(false);
	}

	return(true);
}

