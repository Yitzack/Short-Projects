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
	const vector3 direction[] = {vector3(1,0,0), vector3(-1,0,0), vector3(0,1,0), vector3(0,-1,0), vector3(0,0,1), vector3(0,0,-1), vector3(1,1,0), vector3(1,-1,0), vector3(-1,1,0), vector3(-1,-1,0), vector3(1,0,1), vector3(1,0,-1), vector3(-1,0,1), vector3(-1,0,-1), vector3(0,1,1), vector3(0,1,-1), vector3(0,-1,1), vector3(0,-1,-1), vector3(1,1,1), vector3(-1,1,1), vector3(1,-1,1), vector3(-1,-1,1), vector3(1,1,-1), vector3(-1,1,-1), vector3(1,-1,-1), vector3(-1,-1,-1)};
	vector3 ans(0,0,0);
	vector3 Count(0,0,0);

	for(int i = 0; i < 26; i++)	//Most gradients
	{
		for(int j = 0; j < 26; j++)
		{
			if(Voxel::PermNeighbors[i] != nullptr && Voxel::PermNeighbors[j] != nullptr && (direction[i]-direction[j]).length() < 1.1)
			{
				ans += (func(*dynamic_cast<Air*>(Voxel::PermNeighbors[i]))-func(*dynamic_cast<Air*>(Voxel::PermNeighbors[j])))/Voxel::deltaX*(direction[i]-direction[j]);
				Count += (direction[i]-direction[j]).abs();
			}
		}
	}
	for(int i = 0; i < 6; i++)	//Gradient to center
	{
		if(Voxel::PermNeighbors[i] != nullptr)
		{
			ans += (func(*dynamic_cast<Air*>(Voxel::PermNeighbors[i]))-func(*this))/Voxel::deltaX*(-direction[i]);
			Count += direction[i].abs();
		}
	}

	ans[0] /= Count[0];
	ans[1] /= Count[1];
	ans[2] /= Count[2];

	return(ans);
}

void Air::Store_Neighbor(Voxel* Neighbor, int i)
{
	Voxel::PermNeighbors[i] = Neighbor;
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

