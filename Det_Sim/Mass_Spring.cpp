#include"Mass_Spring.h"

void Mass_Spring::Store_Neighbor(Mass_Spring* Neighbor, int i)
{
	Neighbors[i] = Neighbor;
}

ostream& operator<<(ostream& os, const Mass_Spring& MS)
{
	os << MS.position[0] << "," << MS.position[1] << "," << MS.position[2];
	return(os);
}
