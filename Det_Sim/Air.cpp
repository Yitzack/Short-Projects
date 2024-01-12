#include"Voxel.h"
#include"Air.h"
#include"Mass_Spring.h"

ostream& Air::print(ostream& os) const
{
	Voxel::print(os);
	os << "," << temp << "," << pressure;
	return(os);
}

void Air::Store_Neighbor(Voxel* Neighbor, int i)
{
	if(dynamic_cast<Air*>(Neighbor) != nullptr)
		Voxel::PermNeighbors[i] = Neighbor;
	else
		Voxel::TempNeighbors[i] = Neighbor;
}

