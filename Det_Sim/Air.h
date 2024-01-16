#ifndef AIR_H
#define AIR_H

#include<iostream>
#include<functional>
#include"Voxel.h"
#include"Vector3.h"
using namespace std;

class Air : public Voxel
{
	public:
		Air(int Index[3], bool air) : Voxel(Index)
		{
			if(air)	//Initially air voxel
				prev_pressure = pressure = 101325;
			else	//Initially something else voxel
				prev_pressure = pressure = 0;
			prev_temp = temp = 298.15;
		}
		void Advance() override;		//Take the next time step
		ostream& print(ostream& os) const override
		{
			Voxel::print(os);
			os << "," << temp << "," << pressure << "," << .5*Density()*Voxel::velocity.dot_product(Voxel::velocity);
			return(os);
		}
		void Update_Prev() override
		{
			Voxel::Update_Prev();
			prev_pressure = pressure;
			prev_temp = temp;
		}
		void Store_Neighbor(Voxel* Neighbor, int i) override;	//Store address of neighbors for reference, communication, and divergence calculation
		bool Store_Neighbor(Voxel* Neighbor) override;	//Store address of neighbors for reference, communication, and divergence calculation
		vector3 Gradient(function<float(const Air&)>);
		float Num_of_Part() const{return(prev_pressure*Voxel::volume/(k*prev_temp));}	//PV/kT	(number)
		float Energy() const{return(prev_pressure*Voxel::volume);}			//PV=nkT (J)
		float Mass() const{return(Num_of_Part()/N*molar_mass/1000.);}			//kg
		float Density() const{return(Mass()/Voxel::volume);}				//mass/volume (kg/m^3)
		float Specific_Energy() const{return(Energy()/Mass());}				//energy/mass (J/kg, m^2/s^2)
		float Temp() const{return(prev_temp);}						//kelvin
		float Pressure() const{return(prev_pressure);}					//Pa
	private:
		float temp;			//kelvin
		float pressure;			//pressure aka energy density (Pa)
		float prev_temp;		//kelvin
		float prev_pressure;		//pressure aka energy density (Pa)
		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr float molar_mass = 28.97;	//Molar mass of air (g/mol)
		static constexpr float R = 8.31446261815324;	//molar gas constant (J/(K*mol))
		static constexpr float k = 1.380649e-23;	//Boltzmann Constant (J/K)
		static constexpr float N = 6.02214076e23;	//Avogadro Constant (number)
};

inline ostream& operator<<(ostream& os, const Air& A)
{
	return(A.Air::print(os));
}

#endif
