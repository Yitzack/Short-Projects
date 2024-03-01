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
			Health = 1;
			Preception[0] = 20;
			Preception[1] = Preception[2] = 50;
			Partial_Force[0] = Partial_Force[1] = Partial_Force[2] = vector3(0,0,0);
			Count[0] = Count[1] = Count[2] = 0;
			Max_velocity = 5;
		}
		void Accumalate(const Boid&);
		void Update();
		void Render(ostream&) const;
	private:
		vector3 Position[2];
		vector3 Velocity[2];
		vector3 Partial_Force[3];
		double Preception[3];
		int Count[3];
		double Health;
		double Max_velocity;
		void Align(const Boid&);
		void Cohesion(const Boid&);
		void Avoidence(const Boid&);
		vector3 Avoid_wall();
};

#endif
