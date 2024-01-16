#ifndef MASS_SPRING_H
#define MASS_SPRING_H

#include<cmath>
#include<iostream>
#include"Voxel.h"
using namespace std;

class Mass_Spring : public Voxel
{
	public:
		Mass_Spring(int Index[3]) : Voxel(Index)
		{
			prev_thermal_energy = thermal_energy = specific_heat*mass*298.15;
		}
		void Advance() override;		//Take the next time step
		void Store_Neighbor(Voxel* Neighbor, int i) override;	//Store address of neighbors for reference and communication
		bool Store_Neighbor(Voxel* Neighbor) override;	//Store address of neighbors for reference and communication
		ostream& print(ostream& os) const override
		{
			Voxel::print(os);
			os << "," << thermal_energy << "," << energy();
			return(os);
		}
		void Update_Prev() override
		{
			Voxel::Update_Prev();
			prev_thermal_energy = thermal_energy;
		}
		bool Is_Surface();	//Returns true if one or more PermNeighbors is missing, indicating it is part of the surface of a parent object.
		float energy() const{return(mass*(pow(Voxel::velocity[0],2)+pow(Voxel::velocity[1],2)+pow(Voxel::velocity[2],2))/2.);}
		float temp() const{return(thermal_energy/(specific_heat*mass));}	//kelvin
	private:
		float thermal_energy;		//J
		float prev_thermal_energy;	//J
		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr float density = 2400;		//mass/volume (kg/m^3)
		static constexpr float spring_constant = 4e8;	//k=E*A/L (elastic modulus(40GPa)*area/length) (Pa*m)
		static constexpr float spring_break = 6e-6;	//The distance a spring can streach before it yeilds. (Yeild_strength(24 MPa)*voxel_area/spring_constant)
		static constexpr float specific_heat = 1005;	//specific heat of concrete
		static constexpr float mass = density*Voxel::volume;	//kg
};

inline ostream& operator<<(ostream& os, const Mass_Spring& A)
{
	return(A.Mass_Spring::print(os));
}

#endif
