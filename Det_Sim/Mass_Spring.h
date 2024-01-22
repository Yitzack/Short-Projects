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
			prev_position[0] = position[0] = double(Index[0])/100.;
			prev_position[1] = position[1] = double(Index[1])/100.;
			prev_position[2] = position[2] = double(Index[2])/100.;
			velocity[0] = velocity[1] = velocity[2] = 0;
			prev_velocity[0] = prev_velocity[1] = prev_velocity[2] = 0;
			prev_time = time = 0;
			for(int i = 0; i < 27; i++)
				Neighbors[i/9][(i%9)/3][i%3] = nullptr;
			prev_thermal_energy = thermal_energy = specific_heat*mass*298.15;
		}
		void Advance();		//Take the next time step
		void Store_Neighbor(Mass_Spring*, int, int, int);	//Store address of neighbors for reference and communication
		ostream& print(ostream& os) const
		{
			os << position[0] << "," << position[1] << "," << position[2] << "," << thermal_energy << "," << energy();
			return(os);
		}
		void Update_Prev()
		{
			prev_position = position;
			prev_velocity = velocity;
			prev_thermal_energy = thermal_energy;
		}
		bool Is_Surface() const;	//Returns true if one or more PermNeighbors is missing, indicating it is part of the surface of a parent object.
		vector3 Air_Pressure() const;	//Returns force due to air pressure
		vector3 Position() const;
		void Get_Position(double pos[3]);
		double energy() const{return(mass*(pow(velocity[0],2)+pow(velocity[1],2)+pow(velocity[2],2))/2.);}
		double temp() const{return(thermal_energy/(specific_heat*mass));}	//kelvin
		double Distance(Mass_Spring*) const;
		double Hot_Bubble_Radius() const;
		double Shock_Radius() const;
	private:
		double thermal_energy;		//J
		double prev_thermal_energy;	//J
		double time;			//s
		double prev_time;		//s
		vector3 position;		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		vector3 velocity;		//m/s
		vector3 prev_position;		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		vector3 prev_velocity;		//m/s
		Mass_Spring* Neighbors[3][3][3];	//Addresses of 26 nearest neighbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex) and self

		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr double deltaX = .01;		//Voxel side length (1 cm)
		static constexpr double volume = 1e-6;		//1e-6 is 1cc or 1mL
		static constexpr double deltaT = .001;		//1 ms
		static constexpr double density = 2400;		//mass/volume (kg/m^3)
		static constexpr double spring_constant = 4e8;	//k=E*A/L (elastic modulus(40GPa)*area/length) (Pa*m)
		static constexpr double spring_break = 6e-6;	//The distance a spring can streach before it yeilds. (Yeild_strength(24 MPa)*voxel_area/spring_constant)
		static constexpr double specific_heat = 1005;	//specific heat of concrete
		static constexpr double mass = density*volume;	//kg
		static constexpr double gamma = 5./3.;		//Specific heat ratio
		static constexpr double Gamma=8.*M_PI/3.*gamma/(pow(gamma,2)-1.);
		static constexpr double rho_0 = 1.205;		//Density of air (kg/m^3)
		static constexpr double c_0 = 343.;		//Adiabeatic speed of sound (m/s)
		static constexpr double Energy = 64160928.45;	//Energy in detination of 1kg of TNT and 973.4g of H2O2
		static constexpr double Rf = pow(gamma*Energy/(Gamma*rho_0*pow(c_0,2)),1./3.);
		const vector3 Center = vector3(1.65,1.65,0);
};

inline ostream& operator<<(ostream& os, const Mass_Spring& A)
{
	return(A.Mass_Spring::print(os));
}

#endif
