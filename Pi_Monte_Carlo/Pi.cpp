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
			unsigned long long int Global_Samples_Used[9] = {0,0,0,0,0,0,0,0,0};
			unsigned long long int Global_Points_in_Sphere[9] = {0,0,0,0,0,0,0,0,0};
			double pi[9];
			double Mean, StdDev;

			for(int i = 0; i < omp_get_num_threads(); i++)
			{
				for(int j = 0; j < 9; j++)
				{
					Global_Points_in_Sphere[j] += Points_in_Sphere[i][j];
					Global_Samples_Used[j] += Samples_Used[i][j];
				}
			}

			pi[0] = 4.*double(Global_Points_in_Sphere[0])/double(Global_Samples_Used[0]);
			pi[1] = 6.*double(Global_Points_in_Sphere[1])/double(Global_Samples_Used[1]);
			pi[2] = 4.*sqrt(2.*double(Global_Points_in_Sphere[2])/double(Global_Samples_Used[2]));
			pi[3] = 2.*sqrt(15.*double(Global_Points_in_Sphere[3])/double(Global_Samples_Used[3]));
			pi[4] = 4.*pow(6.*double(Global_Points_in_Sphere[4])/double(Global_Samples_Used[4]),1./3.);
			pi[5] = 2.*pow(105.*double(Global_Points_in_Sphere[5])/double(Global_Samples_Used[5]),1./3.);
			pi[6] = 4.*pow(24.*double(Global_Points_in_Sphere[6])/double(Global_Samples_Used[6]),.25);
			pi[7] = 2.*pow(945.*double(Global_Points_in_Sphere[7])/double(Global_Samples_Used[7]),.25);
			pi[8] = 4.*pow(120.*double(Global_Points_in_Sphere[8])/double(Global_Samples_Used[8]),.2);
			Mean = mean(pi, 9);
			StdDev = stddev(pi, 9);

			cout << Global_Samples_Used[8]+1 << setw(8) << pi[0] << setw(8) << pi[1] << setw(8) << pi[2] << setw(8) << pi[3] << setw(8) << pi[4] << setw(8) << pi[5] << setw(8) << pi[6] << setw(8) << pi[7] << setw(8) << pi[8] << setw(8) << Mean << "+/-" << setw(11) << StdDev << setw(12) << abs(Mean/M_PI-1.) << "+/-" << setw(11) << abs(StdDev/M_PI) << setw(14) << flush;
			//cout << Global_Samples_Used[8]+1 << " " << Global_Points_in_Sphere[0] << " " << Global_Points_in_Sphere[1] << " " << Global_Points_in_Sphere[2] << " " << Global_Points_in_Sphere[3] << " " << Global_Points_in_Sphere[4] << " " << Global_Points_in_Sphere[5] << " " << Global_Points_in_Sphere[6] << " " << Global_Points_in_Sphere[7] << " " << Global_Points_in_Sphere[8] << setw(8) << Mean << "+/-" << setw(11) << StdDev << setw(12) << abs(Mean/M_PI-1.) << "+/-" << setw(11) << abs(StdDev/M_PI) << setw(14) << flush;
			if(Mean-StdDev < M_PI && Mean+StdDev > M_PI)
				cout << "Success" << endl;
			else if(!isnan(StdDev))
				cout << "Fail" << endl;
			else
				cout << "Indeterminate" << endl;	//Would happen if int or float couldn't hold the proper value for standard deviation.
		}
	}while(true);

	return(0);
}
