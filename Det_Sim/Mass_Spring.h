#include<cmath>
#include<iostream>
using namespace std;

#ifndef MASS_H
#define MASS_H

class Mass_Spring
{
	public:
		Mass_Spring(int Index[3])
		{
			position[0] = float(Index[0])/100.;
			position[1] = float(Index[1])/100.;
			position[2] = float(Index[2])/100.;
			thermal_energy = specific_heat*mass*298.15;
			velocity[0] = velocity[1] = velocity[2] = 0;
			for(int i = 0; i < 26; i++)
				Neighbors[i] = nullptr;
		}
		void Advance();		//Take the next time step
		void Store_Neighbor(Mass_Spring*, int);	//Store address of neighbors for reference and communication
		friend ostream& operator<<(ostream&, const Mass_Spring&);
		float energy() const{return(mass*(pow(velocity[0],2)+pow(velocity[1],2)+pow(velocity[2],2))/2.);}
		float temp() const{return(thermal_energy/(specific_heat*mass));}	//kelvin
	private:
		float position[3];		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		float thermal_energy;		//J
		float velocity[3];		//m/s
		Mass_Spring* Neighbors[26];	//Addresses of 26 nearest neightbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex)
		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr float volume = 1e-6;		//1e-6 is 1cc or 1mL
		static constexpr float density = 2400;		//mass/volume (kg/m^3)
		static constexpr float spring_constant = 4e8;	//k=E*A/L (elastic modulus(40GPa)*area/length) (Pa*m)
		static constexpr float spring_break = 6e-6;	//The distance a spring can streach before it yeilds. (Yeild_strength(24 MPa)*voxel_area/spring_constant)
		static constexpr float specific_heat = 1005;	//specific heat of concrete
		static constexpr float mass = density*volume;	//kg
};

#endif
