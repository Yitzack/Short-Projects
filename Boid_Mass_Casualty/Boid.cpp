#include<iostream>
#include<fstream>
#include<cmath>
#include"Vector3.h"
#include"Boid.h"
using namespace std;

void Boid::Align(const Boid& Other)
{
	if((Other.Position[1]-Position[1]).length() > Preception[0] || (Other.Position[1]-Position[1]).length() == 0 || Other.Health == 0)
		return;
	Count[0]++;
	Partial_Force[0] += Other.Velocity[1]-Velocity[1];
	return;
}

void Boid::Cohesion(const Boid& Other)
{
	if((Other.Position[1]-Position[1]).length() > Preception[1] || (Other.Position[1]-Position[1]).length() == 0 || Other.Health == 0)
		return;
	Count[1]++;
	Partial_Force[1] += Other.Position[1]-Position[1];
	return;
}

void Boid::Avoidence(const Boid& Other)
{
	if((Other.Position[1]-Position[1]).length() > Preception[2] || (Other.Position[1]-Position[1]).length() == 0 || Other.Health == 0)
		return;
	Count[2]++;
	Partial_Force[2] += (Position[1]-Other.Position[1]).normalize()/pow((Position[1]-Other.Position[1]).length(),.1);
	return;
}

void Boid::Roost_Force()
{
	Partial_Force[3] = vector3(0,0,-9.8) + Destination-Position[1];	//Drop to the deck and head to the destination
}

void Boid::Render_Aid()
{
	Partial_Force[3] = Destination-Position[1];
}

void Boid::Accumalate(const Boid& Other)
{
	if(State == Flock)
	{
		Align(Other);
		Cohesion(Other);
		Avoidence(Other);
	}
	else
	{
		if((Position[1]-Other.Position[1]).length() < 10 && (Other.State & Render_aid))
			State = States(State | Aid_range);

		if(Role == Civ || Health <= .7)	//Even if EMT, if health is less than .7, then the EMT is unfit for service
		{
			Align(Other);
			Cohesion(Other);
			Avoidence(Other);
			Roost_Force();
		}
		else if(Role == EMT || State == Under_way)
		{
			//Align(Other);
			//Cohesion(Other);
			Avoidence(Other);
			Render_Aid();
		}
	}
}

void Boid::Take_Damage(vector3 Center, double Amplitude)
{
	Health = 1.-Amplitude/pow((Center-Position[1]).length(),1);

	if(Health < 0)
		Health = 0;

	if(Health < .9 && Health > .5)
	{
		if(Role == Civ)
			Max_velocity[0] = MAX_V*2.5*(Health-.5);
		else if(Role == EMT)
			Max_velocity[0] = MAX_V*2.5*(Health-.5)*2.;
	}
	else if(Health > 0 && Health <= .5)
		Max_velocity[0] = 0;
	else if(Health == 0)
		Max_velocity[0] = 1000.;	//Dead, must be non-zero to fall out of sky
}

void Boid::Take_Damage()
{
	if((State & Aid_range) && !(State & (In_care | Under_way)))	//Within range of an EMT rendering aid but not assigned to be in-care of the EMT
		Health = Health;
	else if(Position[1].length() > 10 && !(State & (In_care | Under_way)))
		Health += .01*(Health-.7);	//If the boid is well enough, they can recover on their own. If the damage is too severe, they can go from amblitory to critical or critical to dead.
	else	//Close enough to roost or in the care of a second EMT to heal better
		Health += .01*Health;

	if(Health < 0)
		Health = 0;
	else if(Health > 1)
		Health = 1;

	if(Health < .9 && Health > .5)
	{
		if(Role == Civ)
			Max_velocity[0] = MAX_V*2.5*(Health-.5);
		else if(Role == EMT)
			Max_velocity[0] = MAX_V*2.5*(Health-.5)*2.;
	}
	else if(Health > 0 && Health <= .5)
		Max_velocity[0] = 0;
	else if(Health == 0)
		Max_velocity[0] = 1000.;	//Dead, must be non-zero to fall out of sky
}

vector3 Boid::Avoid_wall()
{
	vector3 Result(0,0,0);

	if(Position[1][0] < 20)
		Result[0] = 1./Position[1][0];
	else if(Position[1][0] > 180)
		Result[0] = 1./(Position[1][0]-200.);

	if(Position[1][1] < 20)
		Result[1] = 1./Position[1][1];
	else if(Position[1][1] > 180)
		Result[1] = 1./(Position[1][1]-200.);

	if(Position[1][2] < 20)
		Result[2] = 1./Position[1][2];
	else if(Position[1][2] > 180)
		Result[2] = 1./(Position[1][2]-200.);

	return(Result);
}

void Boid::Update()
{
	double Max_v;
	vector3 Aligning_force = (Count[0] == 0)?vector3(0,0,0):Partial_Force[0]/Count[0];
	vector3 Cohesion_force = (Count[0] == 0)?vector3(0,0,0):Partial_Force[1]/Count[1];
	vector3 Avoidence_force = (Count[0] == 0)?vector3(0,0,0):Partial_Force[2]/Count[2];
	vector3 Acceleration = Aligning_force + Cohesion_force + 25.*Avoidence_force + Avoid_wall() + Partial_Force[3];
	if(Health == 0)
		Acceleration = vector3(0,0,-9.8);	//Dead, fall out of sky

	Velocity[0] = Velocity[1] + Acceleration;
	if(State & Under_way)	//Max_velocity determined by the EMTs present, can only go as fast as the slower one
		Max_v = Max_velocity[1];
	else	//Max_velocity determined by own health
		Max_v = Max_velocity[0];

	if(Velocity[0].length() > Max_v)
		Velocity[0] = Velocity[0].normalize()*Max_v;
	Position[0] = Position[1] + (Velocity[0]+Velocity[1])/2.;

	if(Position[0][0] < 0)
		Position[0][0] = -Position[0][0];
	else if(Position[0][0] > 200)
		Position[0][0] = 400-Position[0][0];

	if(Position[0][1] < 0)
		Position[0][1] = -Position[0][1];
	else if(Position[0][1] > 200)
		Position[0][1] = 400-Position[0][1];

	if(Position[0][2] < 0 && Health != 0)
		Position[0][2] = -Position[0][2];
	else if(Position[0][2] < 0 && Health == 0)	//If dead and you pass through the deck, stop moving
	{
		Position[0][2] = 0;
		Velocity[0] = vector3(0,0,0);
	}
	else if(Position[0][2] > 200)
		Position[0][2] = 400-Position[0][2];

	Velocity[1] = Velocity[0];
	Position[1] = Position[0];

	Partial_Force[0] = Partial_Force[1] = Partial_Force[2] = Partial_Force[3] = vector3(0,0,0);
	Count[0] = Count[1] = Count[2] = 0;

	if(State & Aid_range)
		State = States(State ^ Aid_range);	//Reset Aid_range state flag for next step
}

void Boid::Render(ostream& os) const
{
	os << Position[0][0] << " " << Position[0][1] << " " << Position[0][2] << " " << Health << endl;
}
