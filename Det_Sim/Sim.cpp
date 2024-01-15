#include<iostream>
#include<cmath>
#include"Voxel.h"
#include"Air.h"
#include"Mass_Spring.h"
using namespace std;

void Init(Air****, Mass_Spring****);	//Initialize the Atmosphere and Bunker
void Print(Air****, Mass_Spring****);
void TempNeighbors(Air****, Mass_Spring****);	//Check and set temporary neighbors as Mass_Spring voxels pass through Air voxels

int main()
{
	Air**** Atmo = new Air***[400];
	Mass_Spring**** Bunker = new Mass_Spring***[330];

	Init(Atmo, Bunker);
	TempNeighbors(Atmo, Bunker);

	return(0);
}

void TempNeighbors(Air**** Atmo, Mass_Spring**** Bunker)
{
	int index[3];
	int Air_index[3];
	float Bunker_position[3];

	for(index[0] = 0; index[0] < 330; index[0]++)
	{
		for(index[1] = 0; index[1] < 330; index[1]++)
		{
			for(index[2] = 0; index[2] < 315; index[2]++)
			{
				if(Bunker[index[0]][index[1]][index[2]] != nullptr)
				{
					Bunker[index[0]][index[1]][index[2]]->Position(Bunker_position);
					Air_index[0] = lrint(Bunker_position[0]*100.);
					Air_index[1] = lrint(Bunker_position[1]*100.);
					Air_index[2] = lrint(Bunker_position[2]*100.);
					for(int i = -1; i <= 1; i++)
						for(int j = -1; j <= 1; j++)
							for(int k = -1; k <= 1; k++)
								if(0 <= Air_index[0]+i && Air_index[0]+i < 400 && 0 <= Air_index[1]+j && Air_index[1]+j < 400 && 0 <=Air_index[2]+k && Air_index[2]+k < 400 && Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[Air_index[0]+i][Air_index[1]+j][Air_index[2]+k]))
									Atmo[Air_index[0]+i][Air_index[1]+j][Air_index[2]+k]->Store_Neighbor(Bunker[index[0]][index[1]][index[2]]);
				}
			}
		}
	}
}

void Print(Air**** Atmo, Mass_Spring**** Bunker)
{
	int index[3];

	for(index[0] = 0; index[0] < 330; index[0]++)	//print all data
	{
		for(index[1] = 0; index[1] < 330; index[1]++)
		{
			for(index[2] = 0; index[2] < 315; index[2]++)
			{
				if(Bunker[index[0]][index[1]][index[2]] != nullptr)
					cout << *Bunker[index[0]][index[1]][index[2]] << endl;
			}
		}
	}
	for(index[0] = 0; index[0] < 400; index[0]++)	//print all data
	{
		for(index[1] = 0; index[1] < 400; index[1]++)
		{
			for(index[2] = 0; index[2] < 400; index[2]++)
			{
				cout << *Atmo[index[0]][index[1]][index[2]] << endl;
			}
		}
	}
}

void Init(Air**** Atmo, Mass_Spring**** Bunker)
{
	int index[3];
	int Num_Masses = 0;
	int Vacuum = 0;
	int Air_Block = 0;

	for(index[0] = 0; index[0] < 400; index[0]++)	//Instantiate all voxels in the same loops
	{
		Atmo[index[0]] = new Air**[400];
		if(index[0] < 330) Bunker[index[0]] = new Mass_Spring**[330];
		for(index[1] = 0; index[1] < 400; index[1]++)
		{
			Atmo[index[0]][index[1]] = new Air*[400];
			if(index[0] < 330 && index[1] < 330) Bunker[index[0]][index[1]] = new Mass_Spring*[315];
			for(index[2] = 0; index[2] < 400; index[2]++)
			{
				if(index[0] < 330 && index[1] < 330 && index[2] < 315 && (index[0] < 15 || index[1] < 15 || index[0] >= 315 || index[1] >= 315 || index[2] >= 300))
					Bunker[index[0]][index[1]][index[2]] = new Mass_Spring(index);
				else if(index[0] < 330 && index[1] < 330 && index[2] < 315)
					Bunker[index[0]][index[1]][index[2]] = nullptr;

				if(index[2] >= 315)	//Above bunker
					Atmo[index[0]][index[1]][index[2]] = new Air(index, true);	//This air voxel is unoccupied
				else if((index[0] >= 35 && index[0] < 365) && (index[1] >= 35 && index[1] < 365) && (index[2] >= 300))	//Ceiling
					Atmo[index[0]][index[1]][index[2]] = new Air(index, false);	//This air voxel is occupided by Bunker
				else if(index[2] >= 300)	//Not ceiling
					Atmo[index[0]][index[1]][index[2]] = new Air(index, true);	//This air voxel is unoccupied
				else if((index[0] >= 35 && index[0] < 50) && (index[1] >= 35 && index[1] < 365))	//Walls
					Atmo[index[0]][index[1]][index[2]] = new Air(index, false);	//This air voxel is occupided by Bunker
				else if((index[0] >= 350 && index[0] < 365) && (index[1] >= 35 && index[1] < 365))
					Atmo[index[0]][index[1]][index[2]] = new Air(index, false);	//This air voxel is occupided by Bunker
				else if((index[1] >= 35 && index[1] < 50) && (index[0] >= 35 && index[0] < 365))
					Atmo[index[0]][index[1]][index[2]] = new Air(index, false);	//This air voxel is occupided by Bunker
				else if((index[1] >= 350 && index[1] < 365) && (index[0] >= 35 && index[0] < 365))
					Atmo[index[0]][index[1]][index[2]] = new Air(index, false);	//This air voxel is occupided by Bunker
				else	//Not walls
					Atmo[index[0]][index[1]][index[2]] = new Air(index, true);	//This air voxel is unoccupied
			}
		}
	}

	for(index[0] = 0; index[0] < 400; index[0]++)	//Find all neighbors
	{
		for(index[1] = 0; index[1] < 400; index[1]++)
		{
			for(index[2] = 0; index[2] < 400; index[2]++)
			{
				if(index[0] < 330 && index[1] < 330 && index[2] < 315 && Bunker[index[0]][index[1]][index[2]] != nullptr)	//Don't over run the alloted indices or bother with null pointers
				{//If no neighbor in that direction due to unavailble index, don't add the neighbor
					if(index[0]+1 < 330) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]+1][index[1]][index[2]],0);
					if(index[0]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]-1][index[1]][index[2]],1);
					if(index[1]+1 < 330) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]][index[1]+1][index[2]],2);
					if(index[1]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]][index[1]-1][index[2]],3);
					if(index[2]+1 < 315) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]][index[1]][index[2]+1],4);
					if(index[2]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]][index[1]][index[2]-1],5);

					if(index[0]+1 < 330 && index[1]+1 < 330) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]+1][index[1]+1][index[2]],6);
					if(index[0]+1 < 330 && index[1]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]+1][index[1]-1][index[2]],7);
					if(index[0]-1 >= 0 && index[1]+1 < 330) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]-1][index[1]+1][index[2]],8);
					if(index[0]-1 >= 0 && index[1]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]-1][index[1]-1][index[2]],9);
					if(index[0]+1 < 330 && index[2]+1 < 315) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]+1][index[1]][index[2]+1],10);
					if(index[0]+1 < 330 && index[2]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]+1][index[1]][index[2]-1],11);
					if(index[0]-1 >= 0 && index[2]+1 < 315) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]-1][index[1]][index[2]+1],12);
					if(index[0]-1 >= 0 && index[2]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]-1][index[1]][index[2]-1],13);
					if(index[1]+1 < 330 && index[2]+1 < 315) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]][index[1]+1][index[2]+1],14);
					if(index[1]+1 < 330 && index[2]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]][index[1]+1][index[2]-1],15);
					if(index[1]-1 >= 0 && index[2]+1 < 315) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]][index[1]-1][index[2]+1],16);
					if(index[1]-1 >= 0 && index[2]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]][index[1]-1][index[2]-1],17);

					if(index[0]+1 < 330 && index[1]+1 < 330 && index[2]+1 < 315) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]+1][index[1]+1][index[2]+1],18);
					if(index[0]-1 >= 0 && index[1]+1 < 330 && index[2]+1 < 315) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]-1][index[1]+1][index[2]+1],19);
					if(index[0]+1 < 330 && index[1]-1 >= 0 && index[2]+1 < 315) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]+1][index[1]-1][index[2]+1],20);
					if(index[0]-1 >= 0 && index[1]-1 >= 0 && index[2]+1 < 315) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]-1][index[1]-1][index[2]+1],21);
					if(index[0]+1 < 330 && index[1]+1 < 330 && index[2]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]+1][index[1]+1][index[2]-1],22);
					if(index[0]-1 >= 0 && index[1]+1 < 330 && index[2]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]-1][index[1]+1][index[2]-1],23);
					if(index[0]+1 < 330 && index[1]-1 >= 0 && index[2]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]+1][index[1]-1][index[2]-1],24);
					if(index[0]-1 >= 0 && index[1]-1 >= 0 && index[2]-1 >= 0) Bunker[index[0]][index[1]][index[2]]->Store_Neighbor(Bunker[index[0]-1][index[1]-1][index[2]-1],25);
				}

				//Repeat for Atmo
				if(index[0]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]+1][index[1]][index[2]],0);
				if(index[0]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]-1][index[1]][index[2]],1);
				if(index[1]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]][index[1]+1][index[2]],2);
				if(index[1]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]][index[1]-1][index[2]],3);
				if(index[2]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]][index[1]][index[2]+1],4);
				if(index[2]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]][index[1]][index[2]-1],5);
				if(index[0]+1 < 400 && index[1]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]+1][index[1]+1][index[2]],6);
				if(index[0]+1 < 400 && index[1]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]+1][index[1]-1][index[2]],7);
				if(index[0]-1 >= 0 && index[1]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]-1][index[1]+1][index[2]],8);
				if(index[0]-1 >= 0 && index[1]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]-1][index[1]-1][index[2]],9);
				if(index[0]+1 < 400 && index[2]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]+1][index[1]][index[2]+1],10);
				if(index[0]+1 < 400 && index[2]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]+1][index[1]][index[2]-1],11);
				if(index[0]-1 >= 0 && index[2]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]-1][index[1]][index[2]+1],12);
				if(index[0]-1 >= 0 && index[2]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]-1][index[1]][index[2]-1],13);
				if(index[1]+1 < 400 && index[2]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]][index[1]+1][index[2]+1],14);
				if(index[1]+1 < 400 && index[2]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]][index[1]+1][index[2]-1],15);
				if(index[1]-1 >= 0 && index[2]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]][index[1]-1][index[2]+1],16);
				if(index[1]-1 >= 0 && index[2]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]][index[1]-1][index[2]-1],17);
				if(index[0]+1 < 400 && index[1]+1 < 400 && index[2]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]+1][index[1]+1][index[2]+1],18);
				if(index[0]-1 >= 0 && index[1]+1 < 400 && index[2]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]-1][index[1]+1][index[2]+1],19);
				if(index[0]+1 < 400 && index[1]-1 >= 0 && index[2]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]+1][index[1]-1][index[2]+1],20);
				if(index[0]-1 >= 0 && index[1]-1 >= 0 && index[2]+1 < 400) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]-1][index[1]-1][index[2]+1],21);
				if(index[0]+1 < 400 && index[1]+1 < 400 && index[2]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]+1][index[1]+1][index[2]-1],22);
				if(index[0]-1 >= 0 && index[1]+1 < 400 && index[2]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]-1][index[1]+1][index[2]-1],23);
				if(index[0]+1 < 400 && index[1]-1 >= 0 && index[2]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]+1][index[1]-1][index[2]-1],24);
				if(index[0]-1 >= 0 && index[1]-1 >= 0 && index[2]-1 >= 0) Atmo[index[0]][index[1]][index[2]]->Store_Neighbor(Atmo[index[0]-1][index[1]-1][index[2]-1],25);
			}
		}
	}
}
