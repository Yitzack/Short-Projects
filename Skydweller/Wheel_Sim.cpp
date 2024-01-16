#include<iostream>
#include"Wheel.h"
using namespace std;

const Airport KCFO_8 = {39.794005383,-104.565244617,1662.19632,39.793561667,-104.536782467,1673.07768};
const Airport KCFO_26 = {39.793561667,-104.536782467,1673.07768,39.794005383,-104.565244617,1662.19632};
const Airport KCFO_17 = {39.785595717,-104.524237388,1669.2372,39.76363995,-104.524230283,1681.03296};
const Airport KCFO_35 = {39.76363995,-104.524230283,1681.03296,39.785595717,-104.524237388,1669.2372};

int main()
{
	double Plane_init_position[] = {39.794005383,-104.565244617,1662.19632};	//Start of runway 8 at KCFO
	double Plane_init_velocity[] = {0,0,0};						//On the ground with no velocity
	Wheel Main_Wheel(Plane_init_position, Plane_init_velocity, KCFO_8);
	return(0);
}























