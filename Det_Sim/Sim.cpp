#include<iostream>
#include<cmath>
#include"Air.h"
#include"Mass_Spring.h"
using namespace std;

int main()
{
	Air**** Atmo = new Air***[400];
	Mass_Spring**** Bunker = new Mass_Spring***[330];
	int index[3];

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

				if((index[0] > 45 && index[0] <= 60) || (index[1] > 45 && index[1] <= 60) || (index[0] > 340 && index[0] <= 355) || (index[1] > 340 && index[1] <= 355) || (index[2] > 300 && index[2] <= 315))
					Atmo[index[0]][index[1]][index[2]] = new Air(index, false);	//This air voxel is occupided by Bunker
				else
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

	for(index[0] = 0; index[0] < 330; index[0]++)	//print all data
	{
		for(index[1] = 0; index[1] < 330; index[1]++)
		{
			for(index[2] = 0; index[2] < 315; index[2]++)
			{
				if(Bunker[index[0]][index[1]][index[2]] != nullptr)
					cout << Bunker[index[0]][index[1]][index[2]] << endl;
			}
		}
	}
	for(index[0] = 0; index[0] < 400; index[0]++)	//print all data
	{
		for(index[1] = 0; index[1] < 400; index[1]++)
		{
			for(index[2] = 0; index[2] < 400; index[2]++)
			{
				cout << Atmo[index[0]][index[1]][index[2]] << endl;
			}
		}
	}

	return(0);
}
