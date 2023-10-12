#include<iostream>
#include<iomanip>
#include<cmath>
#include<random>
#include<omp.h>
#include<vector>
using namespace std;

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

double Lorentz_Point(double A, vector<double> x, vector<double> x0, double gamma)
{
	double Denominator = 0;
	vector<double>:: iterator it_x0 = x0.begin();
	for(vector<double>::iterator it_x = x.begin(); it_x != x.end(); it_x++)
	{
		Denominator += pow(*it_x-*it_x0,2);
		it_x0++;
	}
	return(A*gamma/(M_PI*(Denominator+pow(gamma,2))));
}

double Lorentz_Surface(double A, vector<double> x, double x0, double gamma)
{
	double Denominator = 0;
	for(vector<double>::iterator it_x = x.begin(); it_x != x.end(); it_x++)
	{
		Denominator += *it_x;
	}
	return(A*gamma/(M_PI*(pow(Denominator-x0,2)+pow(gamma,2))));
}

double f1(vector<double> x)	//int_-1^1 dx int_-1^1 dy f1(x,y) = .966673912888639
{
	vector<double> x0 = {0,0};
	double gamma = .1;
	double A = 2;
	return(Lorentz_Point(A, x, x0, gamma));
}

double f2(vector<double> x)	//int_-1^1 dx int_-1^1 dy f2(x,y) = .1809247246180574
{
	vector<double> x0 = {0,0};
	vector<double> x1 = {.5,.5};
	double gamma = .1;
	double A = 2;
	return(Lorentz_Point(A, x, x0, gamma)*Lorentz_Point(A, x, x1, gamma));
}

double f3(vector<double> x)	//int_-1^1 dx int_-1^1 dy f3(x,y) = 1.842829402085427
{
	vector<double> x0 = {0,0};
	vector<double> x1 = {.5,.5};
	double gamma = .1;
	double A = 2;
	return(Lorentz_Point(A, x, x0, gamma)+Lorentz_Point(A, x, x1, gamma));
}

double f4(vector<double> x)	//int_-1^1 dx int_-1^1 dy f4(x,y) = 3.491194554007413
{
	double x0 = 0;
	double gamma = .1;
	double A = 2;
	return(Lorentz_Surface(A, x, x0, gamma));
}

double f5(vector<double> x)	//int_-1^1 dx int_-1^1 dy f5(x,y) = 1.839868331507421
{
	double x0 = 0;
	double x1 = .5;
	double gamma = .1;
	double A = 2;
	double f1 = Lorentz_Surface(A, x, x0, gamma);
	x[1] *= -1;
	double f2 = Lorentz_Surface(A, x, x1, gamma);
	return(f1*f2);
}

double f6(vector<double> x)	//int_-1^1 dx int_-1^1 dy f6(x,y) = 6.319569491139836
{
	double x0 = 0;
	double x1 = .5;
	double gamma = .1;
	double A = 2;
	double f1 = Lorentz_Surface(A, x, x0, gamma);
	x[1] *= -1;
	double f2 = Lorentz_Surface(A, x, x1, gamma);
	return(f1+f2);
}

double f7(vector<double> x)	//int_-1^1 dx f7(x) = A/pi*(ArcTan((b-x0)/gamma)+ArcTan((x0-a)/gamma)) = (atan(7.5)+atan(12.5))/M_PI = .932397
{
	double A = 1;
	double gamma = .1;
	double x0 = -.25;
	return(Lorentz_Surface(A, x, x0, gamma));
}

double f8(vector<double> x)	//int_{unit 10-cube} dV f8(vec x) = 5.6356 (Multi-dimensional), 5.63568+/-.0254724 (Monte Carlo), 5.6357+/-.0254821 (Adaptive Monte Carlo), 5.64082 (Adaptive Quasi Monte Carlo), 5.63616 (Quasi Monte Carlo)
{
	double A = 1;
	double gamma = .1;
	vector<double> x0 = {.06,.25,.18,.79,-.05,-.08,.86,.52,.83,.97};
	return(Lorentz_Point(A, x, x0, gamma));
}

int main(int argc, char* argv[])
{
	unsigned long long int* Samples_Used;
	double* First_Moment;
	double* Second_Moment;
	double* Mean;
	double* StdDev;
	double Correct;
	int Dims;
	double (*f)(vector<double>);

	if(argc == 2)
	{
		switch(argv[1][0])
		{
			case '1':
				Dims = 2;
				f = f1;
				Correct = .966673912888639;
				break;
			case '2':
				Dims = 2;
				f = f2;
				Correct = .1809247246180574;
				break;
			case '3':
				Dims = 2;
				f = f3;
				Correct = 1.842829402085427;
				break;
			case '4':
				Dims = 2;
				f = f4;
				Correct = 3.491194554007413;
				break;
			case '5':
				Dims = 2;
				f = f5;
				Correct = 1.839868331507421;
				break;
			case '6':
				Dims = 2;
				f = f6;
				Correct = 6.319569491139836;
				break;
			case '7':
				Dims = 1;
				f = f7;
				Correct = (atan(7.5)+atan(12.5))/M_PI;
				break;
			case '8':
				Dims = 10;
				f = f8;
				Correct = 5.6356;
				break;
			default:
				cout << "Give an integer between 1 and 8 to get non-NULL output." << endl;
				return(0);
		}
	}
	else
	{
		cout << "Give an integer between 1 and 8 to get non-NULL output." << endl;
		return(0);
	}

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
			Samples_Used = new unsigned long long int[omp_get_num_threads()];
			First_Moment = new double[omp_get_num_threads()];
			Second_Moment = new double[omp_get_num_threads()];
			Mean = new double[omp_get_num_threads()];
			StdDev = new double[omp_get_num_threads()];
		}
		#pragma omp barrier

		RNG[omp_get_thread_num()].seed(time(NULL)+omp_get_thread_num());
		Uniform[omp_get_thread_num()].param(Archetype.param());
		Samples_Used[omp_get_thread_num()] = 0;
		First_Moment[omp_get_thread_num()] = 0;
		Second_Moment[omp_get_thread_num()] = 0;
	}

	#pragma omp parallel
	do
	{
		vector<double> x;
		x.resize(Dims);
		for(int i = 0; i < Dims; i++)
			x[i] = Uniform[omp_get_thread_num()](RNG[omp_get_thread_num()]);
		double Sample = f(x);

		First_Moment[omp_get_thread_num()] = double(Samples_Used[omp_get_thread_num()])/(double(Samples_Used[omp_get_thread_num()]+1))*First_Moment[omp_get_thread_num()]+1./(double(Samples_Used[omp_get_thread_num()]+1))*Sample;
		Second_Moment[omp_get_thread_num()] = double(Samples_Used[omp_get_thread_num()])/(double(Samples_Used[omp_get_thread_num()]+1))*Second_Moment[omp_get_thread_num()]+1./(double(Samples_Used[omp_get_thread_num()]+1))*pow(Sample,2);
		Samples_Used[omp_get_thread_num()]++;
		Mean[omp_get_thread_num()] = pow(2.,Dims)*First_Moment[omp_get_thread_num()];
		StdDev[omp_get_thread_num()] = pow(2.,Dims)*sqrt((Second_Moment[omp_get_thread_num()]-pow(First_Moment[omp_get_thread_num()],2))/Samples_Used[omp_get_thread_num()]);

		#pragma omp master
		if(Samples_Used[0]%100000==0)
		{
			double Mean_Mean, Mean_StdDev;

			Mean_Mean = mean(Mean, omp_get_num_threads());
			Mean_StdDev = meanDev(StdDev, omp_get_num_threads());

			for(int i = 0; i < omp_get_num_threads(); i++)
			{
				cout << Samples_Used[i] << ",Around[" << Mean[i] << "," << StdDev[i] << "]," << flush;
			}
			cout << "Around[" << Mean_Mean << "," << Mean_StdDev << "],Around[" << abs(Mean_Mean/Correct-1.) << "," << Mean_StdDev/Correct << "]" << endl;
		}
	}while(true);

	return(0);
}
