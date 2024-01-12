#include"Voxel.h"

ostream& Voxel::print(ostream& os) const
{
	os << int(position[0]*100) << "," << int(position[1]*100) << "," << int(position[2]);
	return(os);
}

