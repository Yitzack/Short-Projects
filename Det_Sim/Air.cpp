#include"Voxel.h"
#include"Air.h"
#include"Mass_Spring.h"

void Air::Store_Neighbor(Voxel* Neighbor, int i)
{
	if(dynamic_cast<Air*>(Neighbor) != nullptr)
		Voxel::PermNeighbors[i] = Neighbor;
	else
		Voxel::TempNeighbors[i] = Neighbor;
}

