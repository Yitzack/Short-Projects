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

float Voxel::Distance(Voxel* Other)
{
	float distance = 0;
	for(int i = 0; i < 3; i++)
		distance += pow(position[i]-Other->position[i],2);
	return(sqrt(distance));
}
