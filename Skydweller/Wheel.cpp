#include<cmath>
#include"Wheel.h"
using namespace std;

void Wheel::Advance(vector3 Acceleration)
{
	vector3 Velocity(Plane.velocity.vx, Plane.velocity.vy, Plane.velocity.vz);	//Initial values
	vector3 DeltaR;

	DeltaR = Velocity*DeltaT+Acceleration*pow(DeltaT,2)*.5;	//New or changes in values
	Velocity += Acceleration*DeltaT;
	time += DeltaT;
	Acceleration -= vector3(Plane.acceleration.ax,Plane.acceleration.ay,Plane.acceleration.az);
	if(Plane.weight_on_gear)
	{
		Wheel_position += DeltaR.length()/Wheel_Radius;
		while(Wheel_position > 2.*M_PI)
			Wheel_position -= 2.*M_PI;
	}

	Plane.acceleration.ax += Acceleration[0];	//Update the Plane
	Plane.accelerometer.ax += Acceleration[0];
	Plane.acceleration.ay += Acceleration[1];
	Plane.accelerometer.ay += Acceleration[1];
	Plane.acceleration.az += Acceleration[2];
	Plane.accelerometer.az += Acceleration[2];
	Plane.velocity.vx = Velocity[0];
	Plane.velocity.vy = Velocity[1];
	Plane.velocity.vz = Velocity[2];
	Plane.position.longitude += DeltaR[1]/(Radius_Earth*cos(Plane.position.latitude*M_PI/180.));
	Plane.position.latitude += DeltaR[0]/Radius_Earth;
	Plane.position.altitude += DeltaR[2];
	Set_Weight();
}

void Wheel::Set_Weight()
{
	vector3 position(Plane.position.latitude,Plane.position.longitude,Plane.position.altitude);

	vector3 difference = Location.end-Location.start;
	difference[0] *= Radius_Earth;
	difference[1] *= Radius_Earth*cos(Location.start[0]*M_PI/180.);

	const vector3 along_runway = difference.normalize();
	const vector3 gravity(0,0,-9.79669);
	const vector3 across_runway = along_runway.cross_product(gravity).normalize();
	const vector3 runway_normal = along_runway.cross_product(across_runway).normalize();
	double Distance = runway_normal.dot_product(position-Location.start);

	if(Distance > .05)
		Plane.weight_on_gear = false;
	else
		Plane.weight_on_gear = true;
}

pair<bool, bool> Wheel::Sensor_Read() const
{
	if(!Plane.weight_on_gear)
		return(pair<bool,bool>(false,false));

	pair<bool,bool> result(false,false);
	double position;

	for(int j = 0; j < 4; j++)
	{
		position = Bolt_position[j]+Wheel_position;
		if(position > 2.*M_PI)
			position -= 2.*M_PI;
		if(abs(position-Sensor_position[0]) < .25/3.) //Area of positive detection is a quarter inch across at 3 inches from the center or .08333 radians
			result.first = true;
	}

	for(int j = 0; j < 4; j++)
	{
		position = Bolt_position[j]+Wheel_position;
		if(position > 2.*M_PI)
			position -= 2.*M_PI;
		if(abs(position-Sensor_position[1]) < .25/3.) //Area of positive detection is a quarter inch across at 3 inches from the center or .08333 radians
			result.second = true;
	}

	return(result);
}

double Wheel::Speed() const
{
	return(sqrt(pow(Plane.velocity.vx,2)+pow(Plane.velocity.vy,2)+pow(Plane.velocity.vz,2)));
}

ostream& Wheel::print(ostream& os) const
{
	pair<bool, bool> Sensor_On = Sensor_Read();

	os << time << ",";

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

	os << Plane.position.latitude << "," << Plane.position.longitude << "," << Plane.position.altitude << "," << Plane.velocity.vx << "," << Plane.velocity.vy << "," << Plane.velocity.vz << "," << Plane.acceleration.ax << "," << Plane.acceleration.ay << "," << Plane.acceleration.az << "," << Speed();

	return(os);
}

ostream& operator<<(ostream& os, const Wheel W)
{
	return(W.print(os));
}
