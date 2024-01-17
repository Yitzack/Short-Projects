#include<iostream>
#include"Wheel.h"
#include"Vector3.h"
using namespace std;

const Airport KCFO_8 = {vector3(39.794005383,-104.565244617,1662.19632),vector3(39.793561667,-104.536782467,1673.07768)};
const Airport KCFO_26 = {vector3(39.793561667,-104.536782467,1673.07768),vector3(39.794005383,-104.565244617,1662.19632)};
const Airport KCFO_17 = {vector3(39.785595717,-104.524237388,1669.2372),vector3(39.76363995,-104.524230283,1681.03296)};
const Airport KCFO_35 = {vector3(39.76363995,-104.524230283,1681.03296),vector3(39.785595717,-104.524237388,1669.2372)};
const double Radius_Earth = 6371000;

int main()
{
	Wheel Main_Wheel(KCFO_8.start, vector3(0,0,0), KCFO_8);

	vector3 difference = KCFO_8.end-KCFO_8.start;
	difference[0] *= Radius_Earth;
	difference[1] *= Radius_Earth*cos(KCFO_8.start[0]*M_PI/180.);

	const vector3 along_runway = difference.normalize();
	const vector3 gravity(0,0,-9.79669);
	const vector3 across_runway = along_runway.cross_product(gravity).normalize();
	const vector3 runway_normal = along_runway.cross_product(across_runway).normalize();

	double V_lof = 30.87; //60 knots (30.87 m/s) for a Cessna and 160 knots (82.31 m/s) for a Boeing 777
	double Acc = pow(V_lof,2)/16000.;
	vector3 Acc_ground = along_runway*Acc;	//Acceleration is 1.5 gs pointing from start to end of KCFO_8
	vector3 Acc_air = (runway_normal+along_runway).normalize()*Acc;	//1.5gs 45 degrees up from runway surface

	while(Main_Wheel.Speed() < V_lof)
	{
		cout << Main_Wheel << endl;
		Main_Wheel.Advance(Acc_ground);
	}
	while(Main_Wheel.Speed() < 2.*V_lof)
	{
		cout << Main_Wheel << endl;
		Main_Wheel.Advance(Acc_air);
	}

	return(0);
}

