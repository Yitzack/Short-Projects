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
			temp = 298.15;
			velocity[0] = velocity[1] = velocity[2] = 0;
			energy = 0;
			density = 2400;
			mass = density*volumn;
			thermal_energy = specific_heat*mass*temp;
			for(int i = 0; i < 24; i++)
				Neighbors = NULL;
		}
		Update();		//Update derived values
		Advance();		//Take the next time step
		Store_Neighbor(Mass_Spring*);	//Store address of neighbors for reference and communication
	private:
		float density;			//mass/volume (kg/m^3)
		float position[3];		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		const float volume = 1e-6;	//1e-6 is 1cc or 1mL
		float temp;			//kelvin
		float velocity[3];		//m/s
		float mass;			//kg
		float energy;			//1/2mv^2 (J)
		float thermal_energy;		//c*m*T (J)
		const float spring_constant = 4e8;//k=E*A/L (elastic modulus(40GPa)*area/length) (Pa*m)
		const float spring_break = 6e-6;//The distance a spring can streach before it yeilds. (Yeild_strength(24 MPa)*voxel_area/spring_constant)
		Mass_Spring* Neighbors[24];	//Addresses of 24 nearest neightbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex)
		const float specific_heat = 1005;//specific heat of concrete
		const float k = 1.380649e-23;	//Boltzmann Constant (J/K)
}

#endif
