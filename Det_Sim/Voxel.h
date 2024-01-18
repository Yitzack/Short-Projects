#ifndef VOXEL_H
#define VOXEL_H

#include<iostream>
#include<array>
#include"Vector3.cpp"
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
			for(int i = 0; i < 27; i++)
			{
				PermNeighbors[i/9][(i%9)/3][i%3] = nullptr;
				TempNeighbors[i] = nullptr;
			}
		}
		virtual void Advance() = 0;	//Take the next time step
		virtual void Update_Prev()	//Update previous data to match current data for the next step
		{
			prev_position = position;
			prev_velocity = velocity;
		}
		virtual ostream& print(ostream& os) const
		{
			os << int(position[0]*100) << "," << int(position[1]*100) << "," << int(position[2]);
			return(os);
		}
		virtual void Store_Neighbor(Voxel*, int, int, int) = 0;	//Store address of neighbors for reference, communication, and divergence calculation.
		virtual bool Store_Neighbor(Voxel*) = 0;	//Store address of neighbors for reference, communication, and divergence calculation. Returns if Neighbor was added
		void Position(float[3]);
		vector3 Position();
		float Div();
		float Distance(Voxel*);
	protected:
		vector3 position;		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		vector3 velocity;		//m/s
		vector3 prev_position;		//{0,0,0} is the origin at lower, left, back corner, position (meters)
		vector3 prev_velocity;		//m/s
		Voxel* PermNeighbors[3][3][3];	//Addresses of 26 nearest permanent (same type) neighbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex) and self
		Voxel* TempNeighbors[27];	//Addresses of 27 nearest temporary (different type) neighbors (6 face-to-face, 12 edge-to-edge, 8 vertex-to-vertex, and one centered on self)
		//static constants are part of the code included with a class. As such they don't get copied to every instance of the object.
		static constexpr float deltaX = .01;	//Voxel side length (1 cm)
		static constexpr float volume = 1e-6;	//1e-6 is 1cc or 1mL
		static constexpr float deltaT = .001;	//1 ms
};

/*inline ostream& operator<<(ostream& os, const Voxel& A)
{
	return(A.print(os));
}*/

#endif
