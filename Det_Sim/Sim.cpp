#include<iostream>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<functional>
#include<omp.h>
#include"Voxel.h"
#include"Air.h"
#include"Mass_Spring.h"
using namespace std;

void Init(Air****, Mass_Spring****);	//Initialize the Atmosphere and Bunker
void Print(Air****, Mass_Spring****, int);
void TempNeighbors(Air****, Mass_Spring****);	//Check and set temporary neighbors as Mass_Spring voxels pass through Air voxels
void Advance(Air****, Mass_Spring****);

int main()
{
	Air**** Atmo = new Air***[400];
	Mass_Spring**** Bunker = new Mass_Spring***[330];

	Init(Atmo, Bunker);
	cout << "Link first temporary neighbors (1200%+ CPU usage)" << endl;
	TempNeighbors(Atmo, Bunker);

	for(int i = 0; i <= 10; i++)
	{
		if(i%10 == 0) Print(Atmo, Bunker, i);	//Print data every .01 seconds before advancing
		cout << "Advance a time step (2300%+ CPU usage)" << endl;
		Advance(Atmo, Bunker);		//Advance the simulation one time step
		cout << "Relink temporary neighbors" << endl;
		TempNeighbors(Atmo, Bunker);	//Remove temp neighbors that are too far away, add new temp neighbors that are close enough
	}

	return(0);
}

void Advance(Air**** Atmo, Mass_Spring**** Bunker)
{
	int i,j,k;

	#pragma omp parallel for collapse(3)
	for(i = 0; i < 400; i++)
	{
		for(j = 0; j < 400; j++)
		{
			for(k = 0; k < 400; k++)
			{
				if(i < 330 && j < 330 && k < 315 && Bunker[i][j][k] != nullptr)
					Bunker[i][j][k]->Advance();
				Atmo[i][j][k]->Advance();
			}
		}
	}

	#pragma omp parallel for collapse(3)
	for(i = 0; i < 400; i++)
	{
		for(j = 0; j < 400; j++)
		{
			for(k = 0; k < 400; k++)
			{
				if(i < 330 && j < 330 && k < 315 && Bunker[i][j][k] != nullptr)
					Bunker[i][j][k]->Update_Prev();
				Atmo[i][j][k]->Update_Prev();
			}
		}
	}
}

void TempNeighbors(Air**** Atmo, Mass_Spring**** Bunker)
{
	int index_i, index_j, index_k;

	#pragma omp parallel for collapse(3)
	for(index_i = 0; index_i < 330; index_i++)
	{
		for(index_j = 0; index_j < 330; index_j++)
		{
			for(index_k = 0; index_k < 315; index_k++)
			{
				int Air_index[3];
				float Bunker_position[3];
				if(Bunker[index_i][index_j][index_k] != nullptr)
				{
					Bunker[index_i][index_j][index_k]->Position(Bunker_position);
					Air_index[0] = lrint(Bunker_position[0]*100.);
					Air_index[1] = lrint(Bunker_position[1]*100.);
					Air_index[2] = lrint(Bunker_position[2]*100.);
					for(int i = -1; i <= 1; i++)
						for(int j = -1; j <= 1; j++)
							for(int k = -1; k <= 1; k++)
								if(0 <= Air_index[0]+i && Air_index[0]+i < 400 && 0 <= Air_index[1]+j && Air_index[1]+j < 400 && 0 <=Air_index[2]+k && Air_index[2]+k < 400 && Bunker[index_i][index_j][index_k]->Store_Neighbor(Atmo[Air_index[0]+i][Air_index[1]+j][Air_index[2]+k]))
									Atmo[Air_index[0]+i][Air_index[1]+j][Air_index[2]+k]->Store_Neighbor(Bunker[index_i][index_j][index_k]);
				}
			}
		}
	}
}

void Print(Air**** Atmo, Mass_Spring**** Bunker, int Frame)
{
	int i, j, k;
	char File[50] = "Frames/Frame";
	char Number[6];
	sprintf(Number, "%d", Frame);
	strcat(File, Number);
	strcat(File, ".csv");
cout << File << endl;
	ofstream fout(File);

	for(i = 0; i < 330; i++)	//print all data
	{
		for(j = 0; j < 330; j++)
		{
			for(k = 0; k < 315; k++)
			{
				if(Bunker[i][j][k] != nullptr && Bunker[i][j][k]->Is_Surface())
					fout << *Bunker[i][j][k] << endl;
			}
		}
	}
	/*for(i = 0; i < 400; i++)	//print all data
	{
		for(j = 0; j < 400; j++)
		{
			for(k = 0; k < 400; k++)
			{
				if((Atmo[i][j][k]->Gradient(bind(&Air::Pressure,placeholders::_1))).length() > 101325./.01/2.)	//If the gradient is greater than .5 atm per voxel length
					fout << *Atmo[i][j][k] << endl;
			}
		}
	}*/
}

void Init(Air**** Atmo, Mass_Spring**** Bunker)
{
	int i;
cout << "Allocate Memory (~225% CPU usage)" << endl;
	#pragma omp parallel for
	for(i = 0; i < 400; i++)	//Instantiate all voxels in the same loops
	{
		int j, k;
		Atmo[i] = new Air**[400];
		if(i < 330) Bunker[i] = new Mass_Spring**[330];
		for(j = 0; j < 400; j++)
		{
			Atmo[i][j] = new Air*[400];
			if(i < 330 && j < 330) Bunker[i][j] = new Mass_Spring*[315];
			for(k = 0; k < 400; k++)
			{
				int index[] = {i, j, k};

				if(i < 330 && j < 330 && k < 315 && (i < 15 || j < 15 || i >= 315 || j >= 315 || k >= 300))
					Bunker[i][j][k] = new Mass_Spring(index);
				else if(i < 330 && j < 330 && k < 315)
					Bunker[i][j][k] = nullptr;

				if(k >= 315)	//Above bunker
					Atmo[i][j][k] = new Air(index, true);	//This air voxel is unoccupied
				else if((i >= 35 && i < 365) && (j >= 35 && j < 365) && (k >= 300))	//Ceiling
					Atmo[i][j][k] = new Air(index, false);	//This air voxel is occupided by Bunker
				else if(k >= 300)	//Not ceiling
					Atmo[i][j][k] = new Air(index, true);	//This air voxel is unoccupied
				else if((i >= 35 && i < 50) && (j >= 35 && j < 365))	//Walls
					Atmo[i][j][k] = new Air(index, false);	//This air voxel is occupided by Bunker
				else if((i >= 350 && i < 365) && (j >= 35 && j < 365))
					Atmo[i][j][k] = new Air(index, false);	//This air voxel is occupided by Bunker
				else if((j >= 35 && j < 50) && (i >= 35 && i < 365))
					Atmo[i][j][k] = new Air(index, false);	//This air voxel is occupided by Bunker
				else if((j >= 350 && j < 365) && (i >= 35 && i < 365))
					Atmo[i][j][k] = new Air(index, false);	//This air voxel is occupided by Bunker
				else	//Not walls
					Atmo[i][j][k] = new Air(index, true);	//This air voxel is unoccupied
			}
		}
	}
cout << "Link permanent neighbors (1200%+ CPU usage)" << endl;
	int j, k;
	#pragma omp parallel for collapse(3)
	for(i = 0; i < 400; i++)	//Find all neighbors
	{
		for(j = 0; j < 400; j++)
		{
			for(k = 0; k < 400; k++)
			{
				if(i < 330 && j < 330 && k < 315 && Bunker[i][j][k] != nullptr)	//Don't over run the alloted indices or bother with null pointers
				{//If no neighbor in that direction due to unavailble index, don't add the neighbor
					if(i+1 < 330) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j][k],0);
					if(i-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j][k],1);
					if(j+1 < 330) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j+1][k],2);
					if(j-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j-1][k],3);
					if(k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j][k+1],4);
					if(k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j][k-1],5);

					if(i+1 < 330 && j+1 < 330) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j+1][k],6);
					if(i+1 < 330 && j-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j-1][k],7);
					if(i-1 >= 0 && j+1 < 330) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j+1][k],8);
					if(i-1 >= 0 && j-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j-1][k],9);
					if(i+1 < 330 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j][k+1],10);
					if(i+1 < 330 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j][k-1],11);
					if(i-1 >= 0 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j][k+1],12);
					if(i-1 >= 0 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j][k-1],13);
					if(j+1 < 330 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j+1][k+1],14);
					if(j+1 < 330 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j+1][k-1],15);
					if(j-1 >= 0 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j-1][k+1],16);
					if(j-1 >= 0 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j-1][k-1],17);

					if(i+1 < 330 && j+1 < 330 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j+1][k+1],18);
					if(i-1 >= 0 && j+1 < 330 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j+1][k+1],19);
					if(i+1 < 330 && j-1 >= 0 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j-1][k+1],20);
					if(i-1 >= 0 && j-1 >= 0 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j-1][k+1],21);
					if(i+1 < 330 && j+1 < 330 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j+1][k-1],22);
					if(i-1 >= 0 && j+1 < 330 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j+1][k-1],23);
					if(i+1 < 330 && j-1 >= 0 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j-1][k-1],24);
					if(i-1 >= 0 && j-1 >= 0 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j-1][k-1],25);
				}

				//Repeat for Atmo
				if(i+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i+1][j][k],0);
				if(i-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i-1][j][k],1);
				if(j+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i][j+1][k],2);
				if(j-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i][j-1][k],3);
				if(k+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i][j][k+1],4);
				if(k-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i][j][k-1],5);
				if(i+1 < 400 && j+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i+1][j+1][k],6);
				if(i+1 < 400 && j-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i+1][j-1][k],7);
				if(i-1 >= 0 && j+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i-1][j+1][k],8);
				if(i-1 >= 0 && j-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i-1][j-1][k],9);
				if(i+1 < 400 && k+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i+1][j][k+1],10);
				if(i+1 < 400 && k-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i+1][j][k-1],11);
				if(i-1 >= 0 && k+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i-1][j][k+1],12);
				if(i-1 >= 0 && k-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i-1][j][k-1],13);
				if(j+1 < 400 && k+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i][j+1][k+1],14);
				if(j+1 < 400 && k-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i][j+1][k-1],15);
				if(j-1 >= 0 && k+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i][j-1][k+1],16);
				if(j-1 >= 0 && k-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i][j-1][k-1],17);
				if(i+1 < 400 && j+1 < 400 && k+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i+1][j+1][k+1],18);
				if(i-1 >= 0 && j+1 < 400 && k+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i-1][j+1][k+1],19);
				if(i+1 < 400 && j-1 >= 0 && k+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i+1][j-1][k+1],20);
				if(i-1 >= 0 && j-1 >= 0 && k+1 < 400) Atmo[i][j][k]->Store_Neighbor(Atmo[i-1][j-1][k+1],21);
				if(i+1 < 400 && j+1 < 400 && k-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i+1][j+1][k-1],22);
				if(i-1 >= 0 && j+1 < 400 && k-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i-1][j+1][k-1],23);
				if(i+1 < 400 && j-1 >= 0 && k-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i+1][j-1][k-1],24);
				if(i-1 >= 0 && j-1 >= 0 && k-1 >= 0) Atmo[i][j][k]->Store_Neighbor(Atmo[i-1][j-1][k-1],25);
			}
		}
	}
}
