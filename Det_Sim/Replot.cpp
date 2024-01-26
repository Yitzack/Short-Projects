#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;

double Hot_Bubble_Radius(double time)
{
	double R = .0848;
	double deltaR, Temp;
	double T = 0;
	const int Steps = 10; //500 ns steps is small enough. No need to do it in 50 ns steps

	const double deltaT = .00000005;
	const double gamma = 5./3.;
	const double c_0 = sqrt(gamma*8.314*293.15/.02896);
	const double rho_0 = 1.205;
	const double Gamma = 8.*M_PI/3.*gamma/(pow(gamma,2)-1.);
	const double Energy = 64160928.45*2;
	const double Rf = pow(gamma*Energy/(Gamma*rho_0*pow(c_0,2)),1./3.);

	if(time <= .000165)	//Initial calculation
	{
		for(T = 0; T < time-deltaT*Steps; T += deltaT*Steps)
		{
			Temp = sqrt((gamma+1.)/(2.*gamma)*pow(Rf/R,3)+(gamma-1.)/(2.*gamma));
			deltaR = 2.*c_0/(gamma+1.)*(Temp-1./Temp);
			R += deltaR*deltaT*Steps;
		}
		Temp = sqrt((gamma+1.)/(2.*gamma)*pow(Rf/R,3)+(gamma-1.)/(2.*gamma));
		deltaR = 2.*c_0/(gamma+1.)*(Temp-1./Temp);
		R += deltaR*(time-T);
		T = time;
	}
	else
	{
		for(T = 0; T < time-deltaT*Steps; T += deltaT*Steps)
		{
			Temp = sqrt((gamma+1.)/(2.*gamma)*pow(Rf/R,3)+(gamma-1.)/(2.*gamma));
			deltaR = 2.*c_0/(gamma+1.)*(Temp-1./Temp);
			R += deltaR*deltaT*Steps;
		}
		Temp = sqrt((gamma+1.)/(2.*gamma)*pow(Rf/R,3)+(gamma-1.)/(2.*gamma));
		deltaR = 2.*c_0/(gamma+1.)*(Temp-1./Temp);
		R += deltaR*(time-T);
		T = time;

		while(T < time)
		{
			Temp = sqrt((gamma+1.)/(2.*gamma)*pow(Rf/R,3)+(gamma-1.)/(2.*gamma));
			deltaR = 2.*c_0/(gamma+1.)*(Temp-1./Temp);
			R += deltaR*deltaT;
			T += deltaT;
		}
	}

	return(R);
}

double Shock_Radius(int Frame)
{
	const double deltaT = .00000005;
	const double gamma = 5./3.;
	const double c_0 = sqrt(gamma*8.314*293.15/.02896);
	const double rho_0 = 1.205;
	const double Gamma = 8.*M_PI/3.*gamma/(pow(gamma,2)-1.);
	const double Energy = 64160928.45*2;
	const double Rf = pow(gamma*Energy/(Gamma*rho_0*pow(c_0,2)),1./3.);

	double time = Frame*deltaT*20;
	double HR = Hot_Bubble_Radius(time);
	double T1 = -(gamma-1.)/2.*HR/Rf;
	double T2 = gamma/sqrt(3.)*atan(sqrt(3.)*HR/Rf/(2.+HR/Rf));
	double T3 = gamma/6.*log((1.+HR/Rf+pow(HR/Rf,2))/pow(1.-HR/Rf,2));
	return((T1+T2+T3)*Rf);
}

int main()
{
	int data;

	for(int Frame = 0; Frame <= 808; Frame++)
	{
		if(Frame < 165)
			data = 165;
		else
			data = Frame;
		cout << "set datafile separator \",\"\n";
		cout << "set terminal pngcairo size 1280,960\n";
		cout << "set output './Frames/Frame_" << Frame << ".png'\n";
		cout << "set label 1 \"t = " << Frame << " μs, Blastwave radius = " << setprecision(3) << Shock_Radius(Frame) << " m\" at screen 0.5, screen 0.95 center\n";
		cout << "set xrange[-1.35:4.65];set yrange[-1.35:4.65];set zrange[0:4]\n";
		cout << "unset key;unset border;unset xtics;unset ytics;unset ztics\n";
		cout << "set isosamples 25,25;set samples 1000\n";
		cout << "set multiplot\n";

		cout << "set origin 0,0;set size .5,.5\n";
		cout << "set view 90,360\n";
		cout << "set lmargin at screen 0; set rmargin at screen .5\n";
		cout << "set bmargin at screen 0; set tmargin at screen .5\n";
		cout << "splot \"Frames/Frame" << data << ".csv\" using 1:2:3 every 101:::0::0 with dots, sqrt(" << setprecision(18) << Shock_Radius(Frame) << "**2-(x-1.65)**2-(y-1.65)**2)\n";

		cout << "set origin 0,.5;set size .5,.5\n";
		cout << "set view 90,90\n";
		cout << "set lmargin at screen 0; set rmargin at screen .5\n";
		cout << "set bmargin at screen .5; set tmargin at screen 1\n";
		cout << "splot \"Frames/Frame" << data << ".csv\" using 1:2:3 every 101:::0::0 with dots, sqrt(" << setprecision(18) << Shock_Radius(Frame) << "**2-(x-1.65)**2-(y-1.65)**2)\n";

		cout << "set origin .5,0;set size .5,.5\n";
		cout << "set view 0,0\n";
		cout << "set lmargin at screen .5; set rmargin at screen 1\n";
		cout << "set bmargin at screen 0; set tmargin at screen .5\n";
		cout << "splot \"Frames/Frame" << data << ".csv\" using 1:2:3 every 101:::0::0 with dots, sqrt(" << setprecision(18) << Shock_Radius(Frame) << "**2-(x-1.65)**2-(y-1.65)**2)\n";

		cout << "set origin .5,.5;set size .5,.5\n";
		cout << "set view 45,315\n";
		cout << "set lmargin at screen .575; set rmargin at screen .925\n";
		cout << "set bmargin at screen .575; set tmargin at screen .925\n";
		cout << "splot \"Frames/Frame" << data << ".csv\" using 1:2:3 every 101:::0::0 with dots, sqrt(" << setprecision(18) << Shock_Radius(Frame) << "**2-(x-1.65)**2-(y-1.65)**2)\n";

		cout << "unset multiplot" << endl;
	}
	return(0);
}
