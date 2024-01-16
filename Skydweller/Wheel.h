#include<iostream>
#include<cstdlib>
#include<cmath>
#include<ctime>
using namespace std;

#ifndef WHEEL_H
#define WHEEL_H

struct INS
{
	struct geo_position
	{
		double latitude;
		double longitude;
		double altitude;	
	} position;
	struct attitude_euler
	{
		double roll;
		double pitch;
		double yaw;
	} attitude;
	struct ground_velocity
	{
		double vx;
		double vy;
		double vz;
	} velocity;
	struct body_angular_rates
	{
		double p;
		double q;
		double r;
	} attitude_rate;
	struct body_acceleration	//Acceleration measured by an accelerometer on the plane
	{
		double ax;
		double ay;
		double az;
	} accelerometer;
	struct ground_acceleration	//Acceleration relative to the ground
	{
		double ax;
		double ay;
		double az;
	} acceleration;
	struct wind_velocity
	{
		double vx;
		double vy;
		double vz;
	} wind;
	bool weight_on_gear;
};

struct Airport
{
	double latitude_start;
	double longitude_start;
	double altitude_start;
	double latitude_end;
	double longitude_end;
	double altitude_end;
};

class Wheel
{
	public:
		Wheel(double Plane_position[3], double Plane_velocity[3], Airport Runway)
		{
			srand(time(nullptr));

			Sensor_position[0] = M_PI/4.;	//Initialize the wheel
			Sensor_position[1] = 3.*M_PI/4.;
			Wheel_position = rand()/RAND_MAX*2.*M_PI;
			Bolt_position[0] = (2.*rand()/RAND_MAX-1.)*M_PI/70.;
			Bolt_position[1] = M_PI/2. + (2.*rand()/RAND_MAX-1.)*M_PI/70.;
			Bolt_position[2] = M_PI + (2.*rand()/RAND_MAX-1.)*M_PI/70.;
			Bolt_position[3] = 3.*M_PI/2. + (2.*rand()/RAND_MAX-1.)*M_PI/70.;

			Location = Runway;	//As in the location of the test

			Plane.position.latitude = Plane_position[0];	//Initialize the plane INS
			Plane.position.longitude = Plane_position[1];
			Plane.position.altitude = Plane_position[2];
			Plane.velocity.vx = Plane_velocity[0];
			Plane.velocity.vy = Plane_velocity[1];
			Plane.velocity.vz = Plane_velocity[2];
			Plane.acceleration.ax = 0;
			Plane.acceleration.ay = 0;
			Plane.acceleration.az = 0;
			Plane.accelerometer.ax = 5e-5;
			Plane.accelerometer.ay = 1.8e-4;
			Plane.accelerometer.az = -9.79669;

			Set_Weight();
		}
		pair<bool, bool> Sensor_Read() const;
		void Set_Weight();		//Determine if the wheel is on the ground and set Plane.weight_on_gear
		void Advance(double[3]);	//Advance the clock with a given acceleration
		ostream& print(ostream& os) const;
	private:
		double Sensor_position[2];
		double Bolt_position[4];
		double Wheel_position;
		Airport Location;
		INS Plane;
};

#endif
