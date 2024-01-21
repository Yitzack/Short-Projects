#include"Voxel.h"
#include"Mass_Spring.h"
#include"Air.h"
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
				if(Voxel::PermNeighbors[i][j][k] != nullptr && (i != 0 && j != 0 && k!= 0))
					Force += spring_constant*((Voxel::Position()-Voxel::PermNeighbors[i][j][k]->Voxel::Position()).normalize())*((Voxel::Position()-Voxel::PermNeighbors[i][j][k]->Voxel::Position()).length()-equilibrium[i][j][k].length());	//I should have a force vector pointing at the mass on the other end of spring whose equilibrium length is set by the initialization. The spring network will mostly hold voxels in place. When the spring network fails, voxels may intersect each other.

	Force += Air_Pressure();

	Acceleration = Force/mass;
	Acceleration += vector3(0,0,-9.80665);	//Add in force of gravity
	Voxel::velocity = Voxel::prev_velocity + Acceleration*Voxel::deltaT;
	Voxel::position = Voxel::prev_position + Voxel::prev_velocity*Voxel::deltaT + Acceleration*pow(Voxel::deltaT,2)/2.;

	if(Voxel::position[3] < 0)	//If a mass is found below the ground, elastically bounce it off the ground (I think this will effectively create a normal force, but I may need to reconsider)
	{
		Voxel::velocity[3] *= -1;
		Voxel::position[3] *= -1;
	}

	return;
}

vector3 Mass_Spring::Air_Pressure() const
{
	vector3 Pressure_Gradient(0,0,0);
	Pressure = 101325.*pow((288.15-Voxel::position[2]*.0065)/288.15,5.2558);

	if(Voxel::PermNeighbors[0][1][1] == nullptr)
		Pressure_Gradient[0] += Elevation_Pressure*.0001;
	if(Voxel::PermNeighbors[2][1][1] == nullptr)
		Pressure_Gradient[0] -= Elevation_Pressure*.0001;
	if(Voxel::PermNeighbors[1][0][1] == nullptr)
		Pressure_Gradient[1] += Elevation_Pressure*.0001;
	if(Voxel::PermNeighbors[1][2][1] == nullptr)
		Pressure_Gradient[1] -= Elevation_Pressure*.0001;
	if(Voxel::PermNeighbors[1][1][0] == nullptr)
		Pressure_Gradient[2] += Elevation_Pressure*.0001;
	if(Voxel::PermNeighbors[1][1][2] == nullptr)
		Pressure_Gradient[2] -= Elevation_Pressure*.0001;
}

bool Mass_Spring::Is_Surface() const
{
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			for(int k = 0; k < 3; k++)
				if(Voxel::PermNeighbors[i][j][k] == nullptr)
					return(true);
	return(false);
}

void Mass_Spring::Store_Neighbor(Voxel* Neighbor, int i, int j, int k)
{
	Voxel::PermNeighbors[i][j][k] = Neighbor;
	return;
}

bool Mass_Spring::Store_Neighbor(Voxel* Neighbor)
{
	if(Voxel::Distance(Neighbor) > sqrt(3.)*.01)	//Too far away to be a neighbor, short circuit to no entry
		return(false);

	int i;
	pair<float,int> Dist;
	Dist.first = 0;
	Dist.second = -1;

	for(i = 0; i < 27; i++)
	{
		if(Voxel::TempNeighbors[i] == Neighbor)	//Provisioned Neighbor is already present, exit
			return(false);

		if(Voxel::TempNeighbors[i] != nullptr && Voxel::Distance(Voxel::TempNeighbors[i]) > sqrt(3.)*.01)	//Too far away to be a neighbor
			Voxel::TempNeighbors[i] = nullptr;

		if(Voxel::TempNeighbors[i] != nullptr && Dist.first < Voxel::Distance(Voxel::TempNeighbors[i]))	//Find the largest distance of remaining neighbors and its index
		{
			Dist.first = Voxel::Distance(Voxel::TempNeighbors[i]);
			Dist.second = i;
		}
	}

	i = 0;
	while(Voxel::TempNeighbors[i] != nullptr && i < 27)	//Find first nullptr or end of array
		i++;

	if(Voxel::TempNeighbors[i] == nullptr)	//Replace first nullptr with provisioned neighbor, should be the more commond branch
		Voxel::TempNeighbors[i] = Neighbor;
	else if(Voxel::Distance(Neighbor) < Dist.first)	//Replace the furthest TempNeighbor with provisioned neighbor if closer
		Voxel::TempNeighbors[Dist.second] = Neighbor;
	else	//Not closer than any pre-existing neighbor and no nullptr to replace
		return(false);

	return(true);
}

