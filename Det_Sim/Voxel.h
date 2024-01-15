#ifndef VOXEL_H
#define VOXEL_H

#include<iostream>
#include<array>
using namespace std;

class Voxel
{
	public:
		Voxel(int Index[3])
		{
			prev_position[0] = position[0] = float(Index[0])/100.;
			prev_position[1] = position[1] = float(Index[1])/100.;
			prev_position[2] = position[2] = float(Index[2])/100.;
			velocity[0] = velocity[1] = velocity[2] = 0;
			prev_velocity[0] = prev_velocity[1] = prev_velocity[2] = 0;
			for(int i = 0; i < 26; i++)
			{
				PermNeighbors[i] = nullptr;
				TempNeighbors[i] = nullptr;
			}
			TempNeighbors[26] = 0;
		}
		virtual void Advance() = 0;		//Take the next time step
		virtual ostream& print(ostream& os) const
		{
			os << int(position[0]*100) << "," << int(position[1]*100) << "," << int(position[2]);
			return(os);
		}
		virtual void Store_Neighbor(Voxel*, int) = 0;	//Store address of neighbors for reference, communication, and divergence calculation.
		virtual bool Store_Neighbor(Voxel*) = 0;	//Store address of neighbors for reference, communication, and divergence calculation. Returns if Neighbor was added
		void Position(float[3]);
		float Distance(Voxel*);
	protected:
		array<float,3> position;		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		array<float,3> velocity;		//m/s
		array<float,3> prev_position;		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		array<float,3> prev_velocity;		//m/s
		Voxel* PermNeighbors[26];		//Addresses of 26 nearest permanent (same type) neighbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex)
		Voxel* TempNeighbors[27];		//Addresses of 27 nearest temporary (different type) neighbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex, and one centered on self)
		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr float volume = 1e-6;	//1e-6 is 1cc or 1mL
};

/*inline ostream& operator<<(ostream& os, const Voxel& A)
{
	return(A.print(os));
}*/

#endif
