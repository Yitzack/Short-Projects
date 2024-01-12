#ifndef AIR_H
#define AIR_H

#include<iostream>
#include"Voxel.h"
using namespace std;

class Air : public Voxel
{
	public:
		Air(int Index[3], bool air) : Voxel(Index)
		{
			if(air)	//Initially air voxel
				pressure = 101325;
			else	//Initially something else voxel
				pressure = 0;
			temp = 298.15;
		}
		void Advance() override {};		//Take the next time step
		ostream& print(ostream& os) const override;
		void Store_Neighbor(Voxel* Neighbor, int i) override;	//Store address of neighbors for reference, communication, and divergence calculation
		float num_of_part() const{return(pressure*Voxel::volume/(k*temp));}	//PV/kT	(number)
		float energy() const{return(pressure*Voxel::volume);}	//PV=nkT (J)
		float mass() const{return(num_of_part()/N*molar_mass/1000.);}	//kg
		float density() const{return(mass()/Voxel::volume);}	//mass/volume (kg/m^3)
	private:
		float temp;			//kelvin
		float pressure;			//pressure aka energy density (Pa)
		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr float molar_mass = 28.97;	//Molar mass of air (g/mol)
		static constexpr float R = 8.31446261815324;	//molar gas constant (J/(K*mol))
		static constexpr float k = 1.380649e-23;	//Boltzmann Constant (J/K)
		static constexpr float N = 6.02214076e23;	//Avogadro Constant (number)
};

/*ostream& operator<<(ostream& os, const Air& A)
{
	return(A.print(os));
}*/

#endif
