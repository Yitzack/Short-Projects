#include<cmath>
#include"Voxel.h"
using namespace std;

float Voxel::Div()
{
	float Second_W[3] = {-.5,0,.5};
	float First_W[2] = {-1,1};
	vector3 ans(0,0,0);
	int i,j;
	int count;

	count = 0;	//X dimension
	if(PermNeighbors[0][1][1] != nullptr && PermNeighbors[2][1][1] != nullptr)	//Not an edge
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(PermNeighbors[0][1+i][1+j] != nullptr && PermNeighbors[2][1+i][1+j] != nullptr)
				{
					ans[0] += Second_W[0]*(PermNeighbors[0][1+i][1+j]->position)[0]+Second_W[2]*(PermNeighbors[2][1+i][1+j]->position)[0];
					count++;
				}
		ans[0] /= count;
	}
	else if(PermNeighbors[2][1][1] != nullptr)	//High edge
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(PermNeighbors[2][1+i][1+j] != nullptr && PermNeighbors[1][1+i][1+j] != nullptr)
				{
					ans[0] += First_W[0]*(PermNeighbors[1][1+i][1+j]->position)[0]+First_W[1]*(PermNeighbors[2][1+i][1+j]->position)[0];
					count++;
				}
		ans[0] /= count;
	}
	else	//Low edge
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(PermNeighbors[0][1+i][1+j] != nullptr && PermNeighbors[1][1+i][1+j] != nullptr)
				{
					ans[0] += First_W[0]*(PermNeighbors[0][1+i][1+j]->position)[0]+First_W[1]*(PermNeighbors[1][1+i][1+j]->position)[0];
					count++;
				}
		ans[0] /= count;
	}


	count = 0;	//Y direction
	if(PermNeighbors[1][0][1] != nullptr && PermNeighbors[1][2][1] != nullptr)
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(PermNeighbors[1+i][0][1+j] != nullptr && PermNeighbors[1+i][2][1+j] != nullptr)
				{
					ans[1] += Second_W[0]*(PermNeighbors[1+i][0][1+j]->position)[1]+Second_W[2]*(PermNeighbors[1+i][2][1+j]->position)[1];
					count++;
				}
		ans[1] /= count;
	}
	else if(PermNeighbors[1][2][1] != nullptr)
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(PermNeighbors[1+i][2][1+j] != nullptr && PermNeighbors[1+i][1][1+j] != nullptr)
				{
					ans[1] += First_W[0]*(PermNeighbors[1+i][1][1+j]->position)[1]+First_W[1]*(PermNeighbors[1+i][2][1+j]->position)[1];
					count++;
				}
		ans[1] /= count;
	}
	else
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(PermNeighbors[1+i][0][1+j] != nullptr && PermNeighbors[1+i][1][1+j] != nullptr)
				{
					ans[1] += First_W[0]*(PermNeighbors[1+i][0][1+j]->position)[1]+First_W[1]*(PermNeighbors[1+i][1][1+j]->position)[1];
					count++;
				}
		ans[1] /= count;
	}


	count = 0;	//Z direction
	if(PermNeighbors[1][1][0] != nullptr && PermNeighbors[1][1][2] != nullptr)
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(PermNeighbors[1+i][1+j][0] != nullptr && PermNeighbors[1+i][1+j][2] != nullptr)
				{
					ans[2] += Second_W[0]*(PermNeighbors[1+i][1+j][0]->position)[2]+Second_W[2]*(PermNeighbors[1+i][1+j][2]->position)[2];
					count++;
				}
		ans[2] /= count;
	}
	else if(PermNeighbors[1][1][2] != nullptr)
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(PermNeighbors[1+i][1+j][2] != nullptr && PermNeighbors[1+i][1+j][1] != nullptr)
				{
					ans[2] += First_W[0]*(PermNeighbors[1+i][1+j][1]->position)[2]+First_W[1]*(PermNeighbors[1+i][1+j][2]->position)[2];
					count++;
				}
		ans[2] /= count;
	}
	else
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(PermNeighbors[1+i][1+j][0] != nullptr && PermNeighbors[1+i][1+j][1] != nullptr)
				{
					ans[2] += First_W[0]*(PermNeighbors[1+i][1+j][0]->position)[2]+First_W[1]*(PermNeighbors[1+i][1+j][1]->position)[2];
					count++;
				}
		ans[2] /= count;
	}
	ans /= deltaX;

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
