#include"Voxel.h"
#include"Mass_Spring.h"
#include"Air.h"

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

		if(Voxel::Distance(Voxel::TempNeighbors[i]) > sqrt(3.)*.01)	//Too far away to be a neighbor
			Voxel::TempNeighbors[i] = nullptr;

		if(Dist.first < Voxel::Distance(Voxel::TempNeighbors[i]))	//Find the largest distance of remaining neighbors and its index
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

