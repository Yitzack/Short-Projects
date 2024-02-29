#include<fstream>
#include"Vector3.h"
using namespace std;

#ifndef BOID_H
#define BOID_H

class Boid
{
	public:
		Boid(vector3 x, vector3 v)
		{
			Position[0] = Position[1] = x;
			Velocity[0] = Velocity[1] = v;
			Acceleration = vector3(0,0,0);
			Health = 1;
			Preception = 200;
			EMT = false;
		}
		void Accumalate(Boid);
		void Update();
		void Render(ostream&);
	private:
		vector3 Position[2];
		vector3 Velocity[2];
		vector3 Acceleration;
		double Health;
		double Preception;
		bool EMT;
};

#endif
