#include<fstream>
#include"Vector3.h"
using namespace std;

#ifndef BOID_H
#define BOID_H

enum States
{
	Flock,
	Roost
};
enum Type
{
	Civ,
	EMT
};

class Boid
{
	public:
		Boid(vector3 x, vector3 v, Type r)
		{
			Position[0] = Position[1] = x;
			Velocity[0] = Velocity[1] = v;
			Health = 1;
			Preception[0] = 20;
			Preception[1] = Preception[2] = 50;
			Partial_Force[0] = Partial_Force[1] = Partial_Force[2] = Partial_Force[3] = vector3(0,0,0);
			Count[0] = Count[1] = Count[2] = 0;
			Max_velocity = MAX_V;
			State = Flock;
			Role = r;
		}
		void Accumalate(const Boid&);
		void Update();
		void Render(ostream&) const;
		void Change_State(const States New_State){State = New_State;}
		void Take_Damage(vector3, double);
		vector3 Pos() const{return(Position[1]);}
		Type role() const{return(Role);}
		States state() const{return(State);}
	private:
		vector3 Position[2];
		vector3 Velocity[2];
		vector3 Partial_Force[4];
		double Preception[3];
		int Count[3];
		double Health;
		double Max_velocity;
		static constexpr double MAX_V = 5;
		States State;
		Type Role;
		void Align(const Boid&);
		void Cohesion(const Boid&);
		void Avoidence(const Boid&);
		void Roost_Force();
		vector3 Avoid_wall();
};

#endif
