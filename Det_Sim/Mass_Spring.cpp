#include"Mass_Spring.h"
#include"Vector3.h"

void Mass_Spring::Advance()
{
	const vector3 equilibrium[3][3][3] = 
		{{{vector3(-.01,-.01,-.01), vector3(-.01,-.01,0), vector3(-.01,-.01,.01)}, {vector3(-.01,0,-.01), vector3(-.01,0,0), vector3(-.01,0,.01)}, {vector3(-.01,.01,-.01), vector3(-.01,.01,0), vector3(-.01,.01,.01)}}, 
		{{vector3(0,-.01,-.01), vector3(0,-.01,0), vector3(0,-.01,.01)}, {vector3(0,0,-.01), vector3(0,0,0), vector3(0,0,.01)}, {vector3(0,.01,-.01), vector3(0,.01,0), vector3(0,.01,.01)}}, 
		{{vector3(.01,-.01,-.01), vector3(.01,-.01,0), vector3(.01,-.01,.01)}, {vector3(.01,0,-.01), vector3(.01,0,0), vector3(.01,0,.01)}, {vector3(.01,.01,-.01), vector3(.01,.01,0), vector3(.01,.01,.01)}}};
	vector3	Force(0,0,0);
	vector3 Acceleration;

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			for(int k = 0; k < 3; k++)
				if(Neighbors[i][j][k] != nullptr && (i != 0 && j != 0 && k!= 0))
					Force += spring_constant*((Position()-Neighbors[i][j][k]->Position()).normalize())*((Position()-Neighbors[i][j][k]->Position()).length()-equilibrium[i][j][k].length());	//I should have a force vector pointing at the mass on the other end of spring whose equilibrium length is set by the initialization. The spring network will mostly hold Mass_Springs in place. When the spring network fails, Mass_Springs may intersect each other.

	Force += Air_Pressure();

	Acceleration = Force/mass;
	Acceleration += vector3(0,0,-9.80665);	//Add in force of gravity
	velocity = prev_velocity + Acceleration*deltaT;
	position = prev_position + prev_velocity*deltaT + Acceleration*pow(deltaT,2)/2.;

	if(position[3] < 0)	//If a mass is found below the ground, elastically bounce it off the ground (I think this will effectively create a normal force, but I may need to reconsider)
	{
		velocity[3] *= -1;
		position[3] *= -1;
	}

	return;
}

vector3 Mass_Spring::Air_Pressure() const
{
	vector3 Pressure_Gradient(0,0,0);
	float Pressure = 101325.*pow((288.15-position[2]*.0065)/288.15,5.2558);

	if(Neighbors[0][1][1] == nullptr)
		Pressure_Gradient[0] += Pressure*.0001;
	if(Neighbors[2][1][1] == nullptr)
		Pressure_Gradient[0] -= Pressure*.0001;
	if(Neighbors[1][0][1] == nullptr)
		Pressure_Gradient[1] += Pressure*.0001;
	if(Neighbors[1][2][1] == nullptr)
		Pressure_Gradient[1] -= Pressure*.0001;
	if(Neighbors[1][1][0] == nullptr)
		Pressure_Gradient[2] += Pressure*.0001;
	if(Neighbors[1][1][2] == nullptr)
		Pressure_Gradient[2] -= Pressure*.0001;

	return(Pressure_Gradient);
}

bool Mass_Spring::Is_Surface() const
{
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			for(int k = 0; k < 3; k++)
				if(Neighbors[i][j][k] == nullptr)
					return(true);
	return(false);
}

void Mass_Spring::Store_Neighbor(Mass_Spring* Neighbor, int i, int j, int k)
{
	Neighbors[i][j][k] = Neighbor;
	return;
}

void Mass_Spring::Set_Position(float pos[3])
{
	pos[0] = position[0];
	pos[1] = position[1];
	pos[2] = position[2];
	return;
}

vector3 Mass_Spring::Position() const
{
	return(position);
}

float Mass_Spring::Distance(Mass_Spring* Other) const
{
	return((position-Other->position).length());
}
