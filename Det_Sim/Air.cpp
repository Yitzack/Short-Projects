#include<functional>
#include"Voxel.h"
#include"Air.h"
#include"Mass_Spring.h"
#include"Vector3.h"
using namespace std;

void Air::Advance()
{
	float density = Density() + (-Voxel::prev_velocity.dot_product(Gradient(bind(&Air::Pressure,placeholders::_1))) - Density()*Voxel::Div())*Voxel::deltaT;
	Voxel::velocity = Voxel::prev_velocity + (-Voxel::prev_velocity*Voxel::Div() - Gradient(bind(&Air::Pressure,placeholders::_1))/Density() + vector3(0,0,-9.8))*Voxel::deltaT;
	float specific_energy = Specific_Energy() + (-Voxel::velocity.dot_product(Gradient(bind(&Air::Specific_Energy,placeholders::_1))) - Pressure()/Density()*Voxel::Div())*Voxel::deltaT;

	pressure = specific_energy*density;
	temp = pressure/(k*density*N*1000./molar_mass);

	return;
}

vector3 Air::Gradient(function<float(const Air&)> func)
{
	float Second_W[3] = {-.5,0,.5};
	float First_W[2] = {-1,1};
	vector3 ans(0,0,0);
	int i,j;
	int count;

	count = 0;	//X dimension
	if(Voxel::PermNeighbors[0][1][1] != nullptr && Voxel::PermNeighbors[2][1][1] != nullptr)	//Not an edge
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(Voxel::PermNeighbors[0][1+i][1+j] != nullptr && Voxel::PermNeighbors[2][1+i][1+j] != nullptr)
				{
					ans[0] += Second_W[0]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[0][1+i][1+j])))+Second_W[2]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[2][1+i][1+j])));
					count++;
				}
		ans[0] /= count;
	}
	else if(Voxel::PermNeighbors[2][1][1] != nullptr)	//High edge
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(Voxel::PermNeighbors[2][1+i][1+j] != nullptr && Voxel::PermNeighbors[1][1+i][1+j] != nullptr)
				{
					ans[0] += First_W[0]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1][1+i][1+j])))+First_W[1]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[2][1+i][1+j])));
					count++;
				}
		ans[0] /= count;
	}
	else	//Low edge
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(Voxel::PermNeighbors[0][1+i][1+j] != nullptr && Voxel::PermNeighbors[1][1+i][1+j] != nullptr)
				{
					ans[0] += First_W[0]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[0][1+i][1+j])))+First_W[1]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1][1+i][1+j])));
					count++;
				}
		ans[0] /= count;
	}


	count = 0;	//Y direction
	if(Voxel::PermNeighbors[1][0][1] != nullptr && Voxel::PermNeighbors[1][2][1] != nullptr)
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(Voxel::PermNeighbors[1+i][0][1+j] != nullptr && Voxel::PermNeighbors[1+i][2][1+j] != nullptr)
				{
					ans[1] += Second_W[0]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][0][1+j])))+Second_W[2]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][2][1+j])));
					count++;
				}
		ans[1] /= count;
	}
	else if(Voxel::PermNeighbors[1][2][1] != nullptr)
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(Voxel::PermNeighbors[1+i][2][1+j] != nullptr && Voxel::PermNeighbors[1+i][1][1+j] != nullptr)
				{
					ans[1] += First_W[0]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][1][1+j])))+First_W[1]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][2][1+j])));
					count++;
				}
		ans[1] /= count;
	}
	else
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(Voxel::PermNeighbors[1+i][0][1+j] != nullptr && Voxel::PermNeighbors[1+i][1][1+j] != nullptr)
				{
					ans[1] += First_W[0]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][0][1+j])))+First_W[1]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][1][1+j])));
					count++;
				}
		ans[1] /= count;
	}


	count = 0;	//Z direction
	if(Voxel::PermNeighbors[1][1][0] != nullptr && Voxel::PermNeighbors[1][1][2] != nullptr)
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(Voxel::PermNeighbors[1+i][1+j][0] != nullptr && Voxel::PermNeighbors[1+i][1+j][2] != nullptr)
				{
					ans[2] += Second_W[0]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][1+j][0])))+Second_W[2]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][1+j][2])));
					count++;
				}
		ans[2] /= count;
	}
	else if(Voxel::PermNeighbors[1][1][2] != nullptr)
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(Voxel::PermNeighbors[1+i][1+j][2] != nullptr && Voxel::PermNeighbors[1+i][1+j][1] != nullptr)
				{
					ans[2] += First_W[0]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][1+j][1])))+First_W[1]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][1+j][2])));
					count++;
				}
		ans[2] /= count;
	}
	else
	{
		for(i = -1; i <= 1; i++)
			for(j = -1; j <= 1; j++)
				if(Voxel::PermNeighbors[1+i][1+j][0] != nullptr && Voxel::PermNeighbors[1+i][1+j][1] != nullptr)
				{
					ans[2] += First_W[0]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][1+j][0])))+First_W[1]*(func(*dynamic_cast<Air*>(Voxel::PermNeighbors[1+i][1+j][1])));
					count++;
				}
		ans[2] /= count;
	}
	ans /= Voxel::deltaX;

	return(ans);
}

void Air::Store_Neighbor(Voxel* Neighbor, int i, int j, int k)
{
	Voxel::PermNeighbors[i][j][k] = Neighbor;
}

bool Air::Store_Neighbor(Voxel* Neighbor)
{	//No short-circuits required as we already checked the other relation
	int i;
	pair<float,int> Dist;
	Dist.first = 0;
	Dist.second = -1;

	for(i = 0; i < 27; i++)
	{
		if(Voxel::TempNeighbors[i] != nullptr && Voxel::Distance(Voxel::TempNeighbors[i]) > sqrt(3.)*.01)	//Too far away to be a neighbor
			Voxel::TempNeighbors[i] = nullptr;

		if(Voxel::TempNeighbors[i] != nullptr && Dist.first < Voxel::Distance(Voxel::TempNeighbors[i]))	//Find the largest distance of remaining neighbors and its index
		{
			Dist.first = Voxel::Distance(Voxel::TempNeighbors[i]);
			Dist.second = i;
		}
	}

	i = 0;
	while(Voxel::TempNeighbors[i] != nullptr && i < 27)	//Find first nullptr or end of array
		i++;

	if(Voxel::TempNeighbors[i] == nullptr)	//Replace first nullptr with provisioned neighbor, should be the more commond branch
		Voxel::TempNeighbors[i] = Neighbor;
	else if(Voxel::Distance(Neighbor) < Dist.first)	//Replace the furthest TempNeighbor with provisioned neighbor if closer
		Voxel::TempNeighbors[Dist.second] = Neighbor;
	else	//Not closer than any pre-existing neighbor and no nullptr to replace (this branch should not be possible)
		return(false);

	return(true);
}

