#include"Air.h"

void Air::Store_Neighbor(Air* Neighbor, int i)
{
	Neighbors[i] = Neighbor;
}

ostream& operator<<(ostream& os, const Air& A)
{
	os << int(A.position[0]*100) << "," << int(A.position[1]*100) << "," << int(A.position[2]) << "," << A.pressure << "," << A.temp;
	return(os);
}
