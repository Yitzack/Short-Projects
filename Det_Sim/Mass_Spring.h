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
			prev_position[0] = position[0] = float(Index[0])/100.;
			prev_position[1] = position[1] = float(Index[1])/100.;
			prev_position[2] = position[2] = float(Index[2])/100.;
			velocity[0] = velocity[1] = velocity[2] = 0;
			prev_velocity[0] = prev_velocity[1] = prev_velocity[2] = 0;
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
		void Set_Position(float pos[3]);
		float energy() const{return(mass*(pow(velocity[0],2)+pow(velocity[1],2)+pow(velocity[2],2))/2.);}
		float temp() const{return(thermal_energy/(specific_heat*mass));}	//kelvin
		float Distance(Mass_Spring*) const;
	private:
		float thermal_energy;		//J
		float prev_thermal_energy;	//J
		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		vector3 position;		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		vector3 velocity;		//m/s
		vector3 prev_position;		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		vector3 prev_velocity;		//m/s
		Mass_Spring* Neighbors[3][3][3];	//Addresses of 26 nearest permanent (same type) neighbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex) and self
		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr float deltaX = .01;	//Voxel side length (1 cm)
		static constexpr float volume = 1e-6;	//1e-6 is 1cc or 1mL
		static constexpr float deltaT = .001;	//1 ms
		static constexpr float density = 2400;		//mass/volume (kg/m^3)
		static constexpr float spring_constant = 4e8;	//k=E*A/L (elastic modulus(40GPa)*area/length) (Pa*m)
		static constexpr float spring_break = 6e-6;	//The distance a spring can streach before it yeilds. (Yeild_strength(24 MPa)*voxel_area/spring_constant)
		static constexpr float specific_heat = 1005;	//specific heat of concrete
		static constexpr float mass = density*volume;	//kg
};

inline ostream& operator<<(ostream& os, const Mass_Spring& A)
{
	return(A.Mass_Spring::print(os));
}

#endif
