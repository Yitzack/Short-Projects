#include"Voxel.h"
#include"Mass_Spring.h"
#include"Air.h"

ostream& Mass_Spring::print(ostream& os) const
{
	Voxel::print(os);
	os << "," << thermal_energy;
	return(os);
}

void Mass_Spring::Store_Neighbor(Voxel* Neighbor, int i)
{
	if(dynamic_cast<Mass_Spring*>(Neighbor) != nullptr)
		Voxel::PermNeighbors[i] = Neighbor;
	else
		Voxel::TempNeighbors[i] = Neighbor;
}

