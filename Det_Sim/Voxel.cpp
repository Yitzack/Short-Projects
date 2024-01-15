#include<cmath>
#include"Voxel.h"
using namespace std;

void Voxel::Position(float pos[3])
{
	pos[0] = position[0];
	pos[1] = position[1];
	pos[2] = position[2];
	return;
}

vector3 Voxel::Position()
{
	return(position);
}

float Voxel::Distance(Voxel* Other)
{
	return((position-Other->position).length());
}
