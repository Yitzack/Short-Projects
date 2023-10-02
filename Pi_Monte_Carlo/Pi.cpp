#include<iostream>
#include<iomanip>
#include<cmath>
#include<random>
using namespace std;

/*
N	V_sphere=XR^N	V_cube=XR^N	pi=Number*Ratio^(1/n)
2	pi		4		4*circle/total
3	4/3pi		8		6*circle/total
4	1/2pi^2	16		4*sqrt(2*circle/total)
5	8/15pi^2	32		2*sqrt(15*circle/total)
6	1/6pi^3	64		4*(6*circle/total)^(1/3)
7	16/105pi^3	128		2*(105*circle/total)^(1/3)
8	1/24pi^4	256		4*(24*circle/total)^(1/4)
9	32/945pi^4	512		2*(945*circle/total)^(1/4)
10	1/120pi^5	1024		4*(120*circle/total)^(1/5)
*/

double mean(double array[], int N)
{
	double answer = 0;
	for(int i = 0; i < N; i++)
		answer += array[i];
	return(answer/double(N));
}

double stddev(double array[], int N)
{
	double answer = 0;
	for(int i = 0; i < N; i++)
		answer += pow(array[i],2);
	return(sqrt(answer/double(N)-pow(mean(array, N),2)));
}

int Samples_in_Sphere(double Sample[10], int N)
{
	int In = 0;
	double distance;

	for(int i = 0; i+N <= 10; i += N)
	{
		distance = 0;
		for(int j = 0; j < N; j++)
		{
			distance += pow(Sample[i+j],2);
		}
		if(distance < 1) In++;
	}
	return(In);
}

int main()
{
	unsigned long long int Samples_Used[9] = {0,0,0,0,0,0,0,0,0};
	unsigned long long int Points_in_Sphere[9] = {0,0,0,0,0,0,0,0,0};
	int Sub_Samples[9] = {5,3,2,2,1,1,1,1,1};
	double Sample[10];
	double pi[9];
	double Mean, StdDev;

	mt19937 RNG(time(NULL));
	uniform_real_distribution<double> Uniform(-1.,1.); //Uniform random between -1 and 1

	do
	{
		for(int i = 0; i < 10; i++)
			Sample[i] = Uniform(RNG);

		for(int N = 2; N <= 10; N++)
		{
			Points_in_Sphere[N-2] += Samples_in_Sphere(Sample, N);
			Samples_Used[N-2] += Sub_Samples[N-2];
		}

		if(Samples_Used[8]%1000000==999999)
		{
			pi[0] = 4.*double(Points_in_Sphere[0])/double(Samples_Used[0]);
			pi[1] = 6.*double(Points_in_Sphere[1])/double(Samples_Used[1]);
			pi[2] = 4.*sqrt(2.*double(Points_in_Sphere[2])/double(Samples_Used[2]));
			pi[3] = 2.*sqrt(15.*double(Points_in_Sphere[3])/double(Samples_Used[3]));
			pi[4] = 4.*pow(6.*double(Points_in_Sphere[4])/double(Samples_Used[4]),1./3.);
			pi[5] = 2.*pow(105.*double(Points_in_Sphere[5])/double(Samples_Used[5]),1./3.);
			pi[6] = 4.*pow(24.*double(Points_in_Sphere[6])/double(Samples_Used[6]),.25);
			pi[7] = 2.*pow(945.*double(Points_in_Sphere[7])/double(Samples_Used[7]),.25);
			pi[8] = 4.*pow(120.*double(Points_in_Sphere[8])/double(Samples_Used[8]),.2);
			Mean = mean(pi, 9);
			StdDev = stddev(pi, 9);

			cout << Samples_Used[8]+1 << setw(8) << pi[0] << setw(8) << pi[1] << setw(8) << pi[2] << setw(8) << pi[3] << setw(8) << pi[4] << setw(8) << pi[5] << setw(8) << pi[6] << setw(8) << pi[7] << setw(8) << pi[8] << setw(8) << Mean << "+/-" << setw(11) << StdDev << setw(12) << abs(Mean/M_PI-1.) << "+/-" << setw(11) << abs(StdDev/M_PI) << setw(14) << flush;
			if(Mean-StdDev < M_PI && Mean+StdDev > M_PI)
				cout << "Success" << endl;
			else if(!isnan(StdDev))
				cout << "Fail" << endl;
			else
				cout << "Indeterminate" << endl;
		}
	}while(true);

	return(0);
}
