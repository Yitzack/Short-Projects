#include"Voxel.h"
#include"Mass_Spring.h"
#include"Air.h"
#include"Vector3.h"

void Mass_Spring::Advance()
{
	const vector3 equilibrium[] = {vector3(.01,0,0), vector3(-.01,0,0), vector3(0,.01,0), vector3(0,-.01,0), vector3(0,0,.01), vector3(0,0,-.01), vector3(.01,.01,0), vector3(.01,-.01,0), vector3(-.01,.01,0), vector3(-.01,-.01,0), vector3(.01,0,.01), vector3(.01,0,-.01), vector3(-.01,0,.01), vector3(-.01,0,-.01), vector3(0,.01,.01), vector3(0,.01,-.01), vector3(0,-.01,.01), vector3(0,-.01,-.01), vector3(.01,.01,.01), vector3(-.01,.01,.01), vector3(.01,-.01,.01), vector3(-.01,-.01,.01), vector3(.01,.01,-.01), vector3(-.01,.01,-.01), vector3(.01,-.01,-.01), vector3(-.01,-.01,-.01)};
	vector3	Force(0,0,0);
	vector3 Acceleration;

	for(int i = 0; i < 26; i++)
		if(Voxel::PermNeighbors[i] != nullptr)
			Force += spring_constant*((Voxel::Position()-Voxel::PermNeighbors[i]->Voxel::Position()).normalize())*((Voxel::Position()-Voxel::PermNeighbors[i]->Voxel::Position()).length()-equilibrium[i].length());	//I should have a force vector pointing at the mass on the other end of spring whose equilibrium length is set by the initialization. The spring network will mostly hold voxels in place. When the spring network fails, voxels may intersect each other.

	//F=∬A​−∇P⋅ndA (int_area dA -\vec\grad P \cdot \hat{n} where \hat{n} is pointing out)

	Acceleration /= mass;
	Voxel::velocity = Voxel::prev_velocity + Acceleration*Voxel::deltaT;
	Voxel::position = Voxel::prev_position + Voxel::prev_velocity*Voxel::deltaT + Acceleration*pow(Voxel::deltaT,2)/2.;
}

void Mass_Spring::Store_Neighbor(Voxel* Neighbor, int i)
{
	Voxel::PermNeighbors[i] = Neighbor;
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

