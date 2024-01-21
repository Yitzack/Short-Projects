#include<iostream>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<omp.h>
#include"Vector3.h"
#include"Mass_Spring.h"
using namespace std;

void Init(Mass_Spring****);	//Initialize the Atmosphere and Bunker
void Print(Mass_Spring****, int);
void Advance(Mass_Spring****);

int main()
{
	Mass_Spring**** Bunker = new Mass_Spring***[330];

	Init(Bunker);

	for(int i = 0; i <= 10000; i++)
	{
		if(i%1000 == 0) Print(Bunker, i/1000);	//Print data every .01 seconds before advancing
		cout << "Advance to time step " << i << endl;
		Advance(Bunker);	//Advance the simulation one time step
	}

	return(0);
}

void Advance(Mass_Spring**** Bunker)
{
	int i,j,k;

	#pragma omp parallel for collapse(3)
	for(i = 0; i < 330; i++)
		for(j = 0; j < 330; j++)
			for(k = 0; k < 315; k++)
				Bunker[i][j][k]->Advance();

	#pragma omp parallel for collapse(3)
	for(i = 0; i < 330; i++)
		for(j = 0; j < 330; j++)
			for(k = 0; k < 315; k++)
				Bunker[i][j][k]->Update_Prev();
}

void Print(Mass_Spring**** Bunker, int Frame)
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
}

void Init(Mass_Spring**** Bunker)
{
	int i;

	#pragma omp parallel for
	for(i = 0; i < 330; i++)	//Instantiate all voxels in the same loops
	{
		int j, k;
		Bunker[i] = new Mass_Spring**[330];
		for(j = 0; j < 330; j++)
		{
			Bunker[i][j] = new Mass_Spring*[315];
			for(k = 0; k < 315; k++)
			{
				int index[] = {i, j, k};

				if(i < 330 && j < 330 && k < 315 && (i < 15 || j < 15 || i >= 315 || j >= 315 || k >= 300))
					Bunker[i][j][k] = new Mass_Spring(index);
				else if(i < 330 && j < 330 && k < 315)
					Bunker[i][j][k] = nullptr;
			}
		}
	}

	int j, k;
	#pragma omp parallel for collapse(3)
	for(i = 0; i < 330; i++)	//Find all neighbors
	{
		for(j = 0; j < 330; j++)
		{
			for(k = 0; k < 330; k++)
			{
				if(Bunker[i][j][k] != nullptr)	//Don't over run the alloted indices or bother with null pointers
				{
					if(i-1 >= 0 && j-1 >= 0 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j-1][k-1],0,0,0);
					if(i-1 >= 0 && j-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j-1][k],0,0,1);
					if(i-1 >= 0 && j-1 >= 0 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j-1][k+1],0,0,2);
					if(i-1 >= 0 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j][k-1],0,1,0);
					if(i-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j][k],0,1,1);
					if(i-1 >= 0 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j][k+1],0,1,2);
					if(i-1 >= 0 && j+1 < 330 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j+1][k-1],0,2,0);
					if(i-1 >= 0 && j+1 < 330) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j+1][k],0,2,1);
					if(i-1 >= 0 && j+1 < 330 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i-1][j+1][k+1],0,2,2);

					if(j-1 >= 0 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j-1][k-1],1,0,0);
					if(j-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j-1][k],1,0,1);
					if(j-1 >= 0 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j-1][k+1],1,0,2);
					if(k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j][k-1],1,1,0);
					Bunker[i][j][k]->Store_Neighbor(Bunker[i][j][k],1,1,1);
					if(k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j][k+1],1,1,2);
					if(j+1 < 330 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j+1][k-1],1,2,0);
					if(j+1 < 330) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j+1][k],1,2,1);
					if(j+1 < 330 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i][j+1][k+1],1,2,2);

					if(i+1 < 330 && j-1 >= 0 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j-1][k-1],2,0,0);
					if(i+1 < 330 && j-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j-1][k],2,0,1);
					if(i+1 < 330 && j-1 >= 0 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j-1][k+1],2,0,2);
					if(i+1 < 330 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j][k-1],2,1,0);
					if(i+1 < 330) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j][k],2,1,1);
					if(i+1 < 330 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j][k+1],2,1,2);
					if(i+1 < 330 && j+1 < 330 && k-1 >= 0) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j+1][k-1],2,2,0);
					if(i+1 < 330 && j+1 < 330) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j+1][k],2,2,1);
					if(i+1 < 330 && j+1 < 330 && k+1 < 315) Bunker[i][j][k]->Store_Neighbor(Bunker[i+1][j+1][k+1],2,2,2);
				}
			}
		}
	}
}
