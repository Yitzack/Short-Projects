#ifndef AIR_H
#define AIR_H

class Air
{
	public:
		Air(int Index[3])
		{
			position[0] = float(Index[0])/100.;
			position[1] = float(Index[1])/100.;
			position[2] = float(Index[2])/100.;
			pressure = 101325;
			temp = 298.15;
			velocity[0] = velocity[1] = velocity[2] = 0;
			num_of_part = pressure*volume/(k*temp);
			energy = pressure*volume;
			mass = num_of_part/N*molar_mass/1000.;
			density = mass/volume;
			for(int i = 0; i < 24; i++)
				Neighbors = NULL;
		}
		Update();		//Update derived values
		Advance();		//Take the next time step
		Store_Neighbor(Air*);	//Store address of neighbors for reference, communication, and divergence calculation
	private:
		float density;			//mass/volume (kg/m^3)
		float num_of_part;		//PV/kT	(number)
		float position[3];		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		const float volume = 1e-6;	//1e-6 is 1cc or 1mL
		float temp;			//kelvin
		float velocity[3];		//m/s
		float mass;			//kg
		float pressure;			//pressure aka energy density (Pa)
		float energy;			//PV=nkT (J)
		Air* Neighbors[24];		//Addresses of 24 nearest neightbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex)
		const float molar_mass = 28.97;	//Molar mass of air (g/mol)
		const float R = 8.31446261815324;//molar gas constant (J/(K*mol))
		const float k = 1.380649e-23;	//Boltzmann Constant (J/K)
		const float N = 6.02214076e23	//Avogadro Constant (number)
}

#endif
