#include<fstream>
#include"Vector3.h"
#include"Boid.h"
using namespace std;

void Boid::Accumalate(Boid Other)
{
}

void Boid::Update()
{
	Velocity[0] = Velocity[1] + Acceleration;
	Position[0] = Position[1] + (Velocity[0]+Velocity[1])/2.;
	Velocity[1] = Velocity[0];
	Position[1] = Position[0];
}

void Boid::Render(ostream& os)
{
	os << Position[0][0] << " " << Position[0][1] << " " << Position[0][2] << endl;
}
