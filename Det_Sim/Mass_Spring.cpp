#include"Voxel.h"
#include"Mass_Spring.h"
#include"Air.h"

void Mass_Spring::Store_Neighbor(Voxel* Neighbor, int i)
{
	if(dynamic_cast<Mass_Spring*>(Neighbor) != nullptr)
		Voxel::PermNeighbors[i] = Neighbor;
	else
		Voxel::TempNeighbors[i] = Neighbor;
}

