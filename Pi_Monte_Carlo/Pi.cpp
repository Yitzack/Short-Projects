#include<iostream>
#include<iomanip>
#include<cmath>
#include<random>
#include<omp.h>
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

double meanDev(double array[], int N)	//When taking the mean of uncertain values, the uncertainity of the mean is sqrt(Sigma DeltaX_i^2)/N
{
	double answer = 0;
	for(int i = 0; i < N; i++)
		answer += pow(array[i],2);
	return(sqrt(answer)/double(N));
}

double stddev(double array[], int N)
{
	double answer = 0;
	for(int i = 0; i < N; i++)
		answer += pow(array[i],2);
	return(sqrt(answer/double(N)-pow(mean(array, N),2)));
}

int Samples_in_Sphere(double Sample[2520], int N)
{
	int In = 0;
	double distance;

	for(int i = 0; i+N <= 2520; i += N)
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
	unsigned long long int** Samples_Used;
	unsigned long long int** Points_in_Sphere;
	int Sub_Samples[9] = {1260,840,630,504,420,360,315,280,252};

	mt19937* RNG;
	uniform_real_distribution<double> Archetype(-1.,1.);
	uniform_real_distribution<double>* Uniform;
	cout << setprecision(18);

	#pragma omp parallel
	{
		#pragma omp master
		{
			RNG = new mt19937[omp_get_num_threads()];	//Mersenne Twister RNG from Random STL library.
			Uniform = new uniform_real_distribution<double>[omp_get_num_threads()];
			Samples_Used = new unsigned long long int*[omp_get_num_threads()];
			Points_in_Sphere = new unsigned long long int*[omp_get_num_threads()];
		}
		#pragma omp barrier

		RNG[omp_get_thread_num()].seed(time(NULL)+omp_get_thread_num());
		Uniform[omp_get_thread_num()].param(Archetype.param());
		Samples_Used[omp_get_thread_num()] = new unsigned long long int[9];
		Points_in_Sphere[omp_get_thread_num()] = new unsigned long long int[9];
		for(int i = 0; i < 9; i++)
		{
			Samples_Used[omp_get_thread_num()][i] = 0;
			Points_in_Sphere[omp_get_thread_num()][i] = 0;
		}
	}

	#pragma omp parallel
	do
	{
		double Sample[2520];
		for(int i = 0; i < 2520; i++)
			Sample[i] = Uniform[omp_get_thread_num()](RNG[omp_get_thread_num()]);

		for(int N = 2; N <= 10; N++)
		{
			Points_in_Sphere[omp_get_thread_num()][N-2] += Samples_in_Sphere(Sample, N);
			Samples_Used[omp_get_thread_num()][N-2] += Sub_Samples[N-2];
		}

		#pragma omp master
		if((Samples_Used[0][8]/252)%1000==999)
		{
			double pi[9][omp_get_num_threads()];
			double Mean[10], StdDev[10];

			for(int i = 0; i < omp_get_num_threads(); i++)
			{
				pi[0][i] = 4.*double(Points_in_Sphere[i][0])/double(Samples_Used[i][0]);
				pi[1][i] = 6.*double(Points_in_Sphere[i][1])/double(Samples_Used[i][1]);
				pi[2][i] = 4.*sqrt(2.*double(Points_in_Sphere[i][2])/double(Samples_Used[i][2]));
				pi[3][i] = 2.*sqrt(15.*double(Points_in_Sphere[i][3])/double(Samples_Used[i][3]));
				pi[4][i] = 4.*pow(6.*double(Points_in_Sphere[i][4])/double(Samples_Used[i][4]),1./3.);
				pi[5][i] = 2.*pow(105.*double(Points_in_Sphere[i][5])/double(Samples_Used[i][5]),1./3.);
				pi[6][i] = 4.*pow(24.*double(Points_in_Sphere[i][6])/double(Samples_Used[i][6]),.25);
				pi[7][i] = 2.*pow(945.*double(Points_in_Sphere[i][7])/double(Samples_Used[i][7]),.25);
				pi[8][i] = 4.*pow(120.*double(Points_in_Sphere[i][8])/double(Samples_Used[i][8]),.2);
			}

			for(int i = 0; i < 9; i++)
			{
				Mean[i] = mean(pi[i], omp_get_num_threads());
				StdDev[i] = stddev(pi[i], omp_get_num_threads());
			}
			Mean[9] = mean(Mean, 9);
			StdDev[9] = meanDev(StdDev, 9);

			cout << Samples_Used[8][0]+1 << ",Around[" << Mean[0] << "," << StdDev[0] << "],Around[" << Mean[1] << "," << StdDev[1] << "],Around[" << Mean[2] << "," << StdDev[2] << "],Around[" << Mean[3] << "," << StdDev[3] << "],Around[" << Mean[4] << "," << StdDev[4] << "],Around[" << Mean[5] << "," << StdDev[5] << "],Around[" << Mean[6] << "," << StdDev[6] << "],Around[" << Mean[7] << "," << StdDev[7] << "],Around[" << Mean[8] << "," << StdDev[8] << "],Around[" << Mean[9] << "," << StdDev[9] << "],Around[" << abs(Mean[9]/M_PI-1.) << "," << abs(StdDev[9]/M_PI) << "]" << flush;
			if(Mean[9]-StdDev[9] < M_PI && Mean[9]+StdDev[9] > M_PI)
				cout << "Success" << endl;
			else if(!isnan(StdDev[9]))
				cout << "Fail" << endl;
			else
				cout << "Indeterminate" << endl;	//Would happen if int or float couldn't hold the proper value for standard deviation.
		}
	}while(true);

	return(0);
}
