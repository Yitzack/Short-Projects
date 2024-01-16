#include<cmath>
#include"Wheel.h"
using namespace std;

void Wheel::Set_Weight()
{
	Plane.weight_on_gear = true;
}

pair<bool, bool> Wheel::Sensor_Read() const
{
	if(!Plane.weight_on_gear)
		return(pair<bool,bool>(false,false));
	return(pair<bool,bool>(true,true));
}

ostream& Wheel::print(ostream& os) const
{
	pair<bool, bool> Sensor_On = Sensor_Read();

	if(Plane.weight_on_gear)
		os << "1,";
	else
		os << "0,";

	if(Sensor_On.first)
		os << "1,";
	else
		os << "0,";

	if(Sensor_On.second)
		os << "1,";
	else
		os << "0,";

	os << sqrt(pow(Plane.velocity.vx,2)+pow(Plane.velocity.vy,2)+pow(Plane.velocity.vz,2));

	return(os);
}

ostream& operator<<(ostream& os, const Wheel& W)
{
	return(W.print(os));
}
