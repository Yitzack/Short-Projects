#include<fstream>
#include"Vector3.h"
using namespace std;

#ifndef BOID_H
#define BOID_H

enum States
{
	Flock = 0,	//Normal flocking behavor
	Roost = 1,	//Goto roost if able unless EMT, then respond to dispatch
	Under_way = 2,	//Civilian in moving under care of EMT, EMT is moving patient
	In_care = 4,	//Civilian is in care of EMT or roost
	Assign1 = 8,	//Civilian has been assigned first EMT
	Assign2 = 16,	//Civilian has been assigned second EMT
	Render_aid = 32,//EMT is on-scene and rendering aid
	Aid_range = 64	//Civilian is within range of an EMT with render_aid set
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
			Destination = vector3(0,0,0);
			Velocity[0] = Velocity[1] = v;
			Health = 1;
			Preception[0] = 20;
			Preception[1] = Preception[2] = 50;
			Partial_Force[0] = Partial_Force[1] = Partial_Force[2] = Partial_Force[3] = vector3(0,0,0);
			Count[0] = Count[1] = Count[2] = 0;
			Max_velocity[0] = Max_velocity[1] = MAX_V;
			State = Flock;
			Role = r;
		}
		void Accumalate(const Boid&);
		void Update();
		void Render(ostream&) const;
		void Take_Damage(vector3, double);	//Firework damage
		void Take_Damage();		//D&D would call it bleeding damage

		bool operator<(const Boid Other) const{return(Health > Other.Health);}	//Reverse the order so that the prioity queue will sort in the reverse order

		void Change_State(const States New_State){State = New_State;}
		void Assign_Destination(const vector3 pos){Destination = pos;}
		void Set_Max_v(const double Max_v){Max_velocity[1] = Max_v;}	//Set the max velocity according to EMTs present

		vector3 Pos() const{return(Position[1]);}
		vector3 Dest() const{return(Destination);}
		double health() const{return(Health);}
		Type role() const{return(Role);}
		States state() const{return(State);}
		double Get_Max_v() const{return(Max_velocity[0]);}
	private:
		vector3 Position[2];
		vector3 Destination;
		vector3 Velocity[2];
		vector3 Partial_Force[4];
		double Preception[3];
		int Count[3];
		double Health;
		double Max_velocity[2];
		static constexpr double MAX_V = 5;
		States State;
		Type Role;
		void Align(const Boid&);
		void Cohesion(const Boid&);
		void Avoidence(const Boid&);
		void Roost_Force();
		void Render_Aid();
		vector3 Avoid_wall();
};

#endif
