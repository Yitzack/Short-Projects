#include<iostream>
#include<cmath>
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

int Samples_in_Sphere(float Sample[10], int N)
{
	int In = 0;
	float distance;

	for(int i = 0; i < 10; i += N)
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
	int Samples_Used[9] = {0,0,0,0,0,0,0,0,0};
	int Points_in_Sphere[9] = {0,0,0,0,0,0,0,0,0};
	int Sub_Samples[9] = {5,3,2,2,1,1,1,1,1};
	float Sample[10];

	do
	{
		for(int i = 0; i < 10; i++)
			Sample[i] = 2.*float(rand())/float(RAND_MAX)-1.;	//Uniform random between -1 and 1, not recommended RGN method

		for(int N = 2; N <= 10; N++)
		{
			Points_in_Sphere[N-2] += Samples_in_Sphere(Sample, N);
			Samples_Used[N-2] += Sub_Samples[N-2];
		}

		if(Samples_Used[8]%1000==999)
			cout << Samples_Used[8]+1 << " " << 4.*float(Points_in_Sphere[0])/float(Samples_Used[0]) << " " << 6.*float(Points_in_Sphere[1])/float(Samples_Used[1]) << " " << 4.*sqrt(2.*float(Points_in_Sphere[2])/float(Samples_Used[2])) << " " << 2.*sqrt(15.*float(Points_in_Sphere[3])/float(Samples_Used[3])) << " " << 4.*pow(6.*float(Points_in_Sphere[4])/float(Samples_Used[4]),1./3.) << " " << 2.*pow(105.*float(Points_in_Sphere[5])/float(Samples_Used[5]),1./3.) << " " << 4.*pow(24.*float(Points_in_Sphere[6])/float(Samples_Used[6]),.25) << " " << 2.*pow(945.*float(Points_in_Sphere[7])/float(Samples_Used[7]),.25) << " " << 4.*pow(120.*float(Points_in_Sphere[8])/float(Samples_Used[8]),.2) << endl;
	}while(Samples_Used[8] < 100000);

	return(0);
}
