#ifndef MASS_SPRING_H
#define MASS_SPRING_H

#include<cmath>
#include<iostream>
#include"Vector3.h"
using namespace std;

class Mass_Spring
{
	public:
		Mass_Spring(int Index[3])
		{
			for(int i = 0; i < 5; i++)
			{
				position[i][0] = double(Index[0])/100.;
				position[i][1] = double(Index[1])/100.;
				position[i][2] = double(Index[2])/100.;
				velocity[i][0] = velocity[i][1] = velocity[i][2] = 0;
				time[i] = 0;
			}
			for(int i = 0; i < 27; i++)
				Neighbors[i/9][(i%9)/3][i%3] = nullptr;
			prev_thermal_energy = thermal_energy = specific_heat*mass*298.15;
		}
		void Advance0();	//Take the 0th partial step
		void Advance1();	//Take the 1st partial step
		void Advance2();	//Take the 2nd partial step
		void Update_Prev();			//Take the 3rd partial step and update 0th results to current results
		void Store_Neighbor(Mass_Spring*, int, int, int);	//Store address of neighbors for reference and communication
		ostream& print(ostream& os) const
		{
			os << position[0][0] << "," << position[0][1] << "," << position[0][2] << "," << thermal_energy << "," << energy();
			return(os);
		}
		void Normal_Force(int);			//Elastically bounces voxels off the floor for the normal force
		bool Is_Surface() const;		//Returns true if one or more PermNeighbors is missing, indicating it is part of the surface of a parent object.
		vector3 Force_f(int) const;		//Returns the force applied to the mass
		vector3 Air_Pressure_Grad(int) const;	//Returns force due to air pressure
		double Air_Pressure(vector3, double) const;	//Returns air pressure
		vector3 Position(int) const;
		void Get_Position(double pos[3]);
		double energy() const{return(mass*(pow(velocity[0][0],2)+pow(velocity[0][1],2)+pow(velocity[0][2],2))/2.);}
		double temp() const{return(thermal_energy/(specific_heat*mass));}	//kelvin
		double Distance(Mass_Spring*) const;
		double Hot_Bubble_Radius(double) const;
		double Shock_Radius(double) const;
	private:
		double thermal_energy;		//J
		double prev_thermal_energy;	//J
		double time[5];			//s
		vector3 position[5];		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		vector3 velocity[5];		//m/s
		Mass_Spring* Neighbors[3][3][3];	//Addresses of 26 nearest neighbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex) and self

		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr double deltaX = .01;		//Voxel side length (1 cm)
		static constexpr double volume = 1e-6;		//1e-6 is 1cc or 1mL
		static constexpr double deltaT = .00000005;	//50 ns
		static constexpr double density = 2400;		//mass/volume (kg/m^3)
		static constexpr double spring_constant = 4e8;	//k=E*A/L (elastic modulus(40GPa)*area/length) (Pa*m)
		static constexpr double spring_break = 6e-6;	//The distance a spring can streach before it yeilds. (Yeild_strength(24 MPa)*voxel_area/spring_constant)
		static constexpr double specific_heat = 1005;	//specific heat of concrete
		static constexpr double mass = density*volume;	//kg
		static constexpr double gamma = 5./3.;		//Specific heat ratio
		static constexpr double Gamma = 8.*M_PI/3.*gamma/(pow(gamma,2)-1.);
		static constexpr double rho_0 = 1.205;		//Density of air (kg/m^3)
		static constexpr double c_0 = sqrt(gamma*8.314*293.15/.02896);	//adiabatic sound speed (m/s)
		static constexpr double Energy = 64160928.45;	//Energy in detination of 1kg of TNT and 973.4g of H2O2
		static constexpr double Rf = pow(gamma*Energy/(Gamma*rho_0*pow(c_0,2)),1./3.);
		const vector3 Center = vector3(1.65,1.65,0);
};

inline ostream& operator<<(ostream& os, const Mass_Spring& A)
{
	return(A.Mass_Spring::print(os));
}

#endif
