#ifndef AIR_H
#define AIR_H

class Air
{
	public:
		Air(int Index[3], bool air)
		{
			position[0] = float(Index[0])/100.;
			position[1] = float(Index[1])/100.;
			position[2] = float(Index[2])/100.;
			if(air)	//Initially air voxel
				pressure = 101325;
			else	//Initially something else voxel
				pressure = 0;
			temp = 298.15;
			velocity[0] = velocity[1] = velocity[2] = 0;
			for(int i = 0; i < 26; i++)
				Neighbors[i] = nullptr;
		}
		void Advance();			//Take the next time step
		void Store_Neighbor(Air*, int);	//Store address of neighbors for reference, communication, and divergence calculation
		float num_of_part() const{return(pressure*volume/(k*temp));}	//PV/kT	(number)
		float energy() const{return(pressure*volume);}	//PV=nkT (J)
		float mass() const{return(num_of_part()/N*molar_mass/1000.);}	//kg
		float density() const{return(mass()/volume);}	//mass/volume (kg/m^3)
	private:
		float position[3];		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		float temp;			//kelvin
		float velocity[3];		//m/s
		float pressure;			//pressure aka energy density (Pa)
		Air* Neighbors[26];		//Addresses of 26 nearest neightbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex)
		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr float volume = 1e-6;		//1e-6 is 1cc or 1mL
		static constexpr float molar_mass = 28.97;	//Molar mass of air (g/mol)
		static constexpr float R = 8.31446261815324;	//molar gas constant (J/(K*mol))
		static constexpr float k = 1.380649e-23;	//Boltzmann Constant (J/K)
		static constexpr float N = 6.02214076e23;	//Avogadro Constant (number)
};

#endif
