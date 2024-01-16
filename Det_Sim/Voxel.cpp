#include<cmath>
#include"Voxel.h"
using namespace std;

float Voxel::Div()
{
	const vector3 direction[] = {vector3(1,0,0), vector3(-1,0,0), vector3(0,1,0), vector3(0,-1,0), vector3(0,0,1), vector3(0,0,-1), vector3(1,1,0), vector3(1,-1,0), vector3(-1,1,0), vector3(-1,-1,0), vector3(1,0,1), vector3(1,0,-1), vector3(-1,0,1), vector3(-1,0,-1), vector3(0,1,1), vector3(0,1,-1), vector3(0,-1,1), vector3(0,-1,-1), vector3(1,1,1), vector3(-1,1,1), vector3(1,-1,1), vector3(-1,-1,1), vector3(1,1,-1), vector3(-1,1,-1), vector3(1,-1,-1), vector3(-1,-1,-1)};
	vector3 ans(0,0,0);
	vector3 Count(0,0,0);

	for(int i = 0; i < 26; i++)	//Most gradients
	{
		for(int j = 0; j < 26; j++)
		{
			if(PermNeighbors[i] != nullptr && PermNeighbors[j] != nullptr && (direction[i]-direction[j]).length() < 1.1)
			{
				ans += (PermNeighbors[i]->prev_velocity-PermNeighbors[i]->prev_velocity).dot_product(direction[i]-direction[j])*(direction[i]-direction[j])/deltaX;	//The first factor of the dot product gets the finite difference of all components. The second factor gets just the part pointing in the correct direction. The last factor turns the scalar into a vector to fit in the correct component.
				Count += (direction[i]-direction[j]).abs();
			}
		}
	}
	for(int i = 0; i < 6; i++)	//Gradient to center
	{
		if(Voxel::PermNeighbors[i] != nullptr)
		{
			ans += (PermNeighbors[i]->prev_velocity-prev_velocity).dot_product(direction[i])*(-direction[i])/deltaX;	//The first factor of the dot product gets the finite difference of all components. The second factor gets just the part pointing in the correct direction. The last factor turns the scalar into a vector to fit in the correct component.
			Count += direction[i].abs();
		}
	}

	ans[0] /= Count[0];
	ans[1] /= Count[1];
	ans[2] /= Count[2];

	return(ans[0]+ans[1]+ans[2]);
}

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
