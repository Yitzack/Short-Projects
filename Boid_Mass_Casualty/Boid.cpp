#include<fstream>
#include<cmath>
#include"Vector3.h"
#include"Boid.h"
using namespace std;

void Boid::Align(const Boid& Other)
{
	if((Other.Position[1]-Position[1]).length() > Preception[0] || (Other.Position[1]-Position[1]).length() == 0)
		return;
	Count[0]++;
	Partial_Force[0] += Other.Velocity[1]-Velocity[1];
	return;
}

void Boid::Cohesion(const Boid& Other)
{
	if((Other.Position[1]-Position[1]).length() > Preception[1] || (Other.Position[1]-Position[1]).length() == 0)
		return;
	Count[1]++;
	Partial_Force[1] += Other.Position[1]-Position[1];
	return;
}

void Boid::Avoidence(const Boid& Other)
{
	if((Other.Position[1]-Position[1]).length() > Preception[2] || (Other.Position[1]-Position[1]).length() == 0)
		return;
	Count[2]++;
	Partial_Force[2] += (Position[1]-Other.Position[1]).normalize()/((Position[1]-Other.Position[1]).length());
	return;
}

void Boid::Accumalate(const Boid& Other)
{
	Align(Other);
	Cohesion(Other);
	Avoidence(Other);
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
	vector3 Aligning_force = (Count[0] == 0)?vector3(0,0,0):Partial_Force[0]/Count[0];
	vector3 Cohesion_force = (Count[0] == 0)?vector3(0,0,0):Partial_Force[1]/Count[1];
	vector3 Avoidence_force = (Count[0] == 0)?vector3(0,0,0):Partial_Force[2]/Count[2];
	vector3 Acceleration = Aligning_force + Cohesion_force + Avoidence_force + Avoid_wall();

	Velocity[0] = Velocity[1] + Acceleration;
	if(Velocity[0].length() > Max_velocity)
		Velocity[0] = Velocity[0].normalize()*Max_velocity;
	Position[0] = Position[1] + (Velocity[0]+Velocity[1])/2.;

	if(Position[0][0] < 0)
		Position[0][0] = -Position[0][0];
	else if(Position[0][0] > 200)
		Position[0][0] = 400-Position[0][0];

	if(Position[0][1] < 0)
		Position[0][1] = -Position[0][1];
	else if(Position[0][1] > 200)
		Position[0][1] = 400-Position[0][1];

	if(Position[0][2] < 0)
		Position[0][2] = -Position[0][2];
	else if(Position[0][2] > 200)
		Position[0][2] = 400-Position[0][2];

	Velocity[1] = Velocity[0];
	Position[1] = Position[0];

	Partial_Force[0] = Partial_Force[1] = Partial_Force[2] = vector3(0,0,0);
	Count[0] = Count[1] = Count[2] = 0;
}

void Boid::Render(ostream& os) const
{
	os << Position[0][0] << " " << Position[0][1] << " " << Position[0][2] << endl;
}
