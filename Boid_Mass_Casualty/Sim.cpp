#include<iostream>
#include<fstream>
#include<vector>
#include<random>
#include<ctime>
#include"Vector3.h"
#include"Boid.h"
using namespace std;

void Render(int i);

int main()
{
	mt19937_64 RNG;
	uniform_real_distribution<double> Uniform(0,200);
	vector<Boid> Flock;
	ofstream Civilian("Frame/Civilian");
	ofstream Emergency("Frame/EMT");
	vector3 pos, vel;
	int i = 0;

	RNG.seed(time(NULL));

	for(i = 0; i < 100; i++)
	{
		pos = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = (vel-vector3(100.,100.,100.))/10.;
		Flock.push_back(Boid(pos, vel, Civ));
	}
	for(i = 0; i < 6; i++)
	{
		pos = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = (vel-vector3(100.,100.,100.))/10.;
		Flock.push_back(Boid(pos, vel, EMT));
	}

	for(Boid& Self: Flock)
		if(Self.role() == Civ) Self.Render(Civilian);
	for(Boid& Self: Flock)
		if(Self.role() == EMT) Self.Render(Emergency);
	Civilian << endl;
	Emergency << endl;
	Render(0);

	for(i = 1; i <= 30; i++)
	{
		for(Boid& Self: Flock)
			for(Boid Other: Flock)
				Self.Accumalate(Other);

		for(Boid& Self: Flock)
		{
			Self.Update();
			if(Self.role() == Civ) Self.Render(Civilian);
			else if(Self.role() == EMT) Self.Render(Emergency);
		}
		Civilian << endl;
		Emergency << endl;
		Render(i);
	}

	uniform_int_distribution<int> Boid_Choice(0, Flock.size()-1);
	pos = Flock[Boid_Choice(RNG)].Pos();
	cerr << "Center: " << pos[0] << "," << pos[1] << "," << pos[2] << endl;

	for(Boid& Self: Flock)
	{
		Self.Change_State(Roost);
		Self.Take_Damage(pos, 10);
	}

	for(i; i <= 100; i++)
	{
		for(Boid& Self: Flock)
			for(Boid Other: Flock)
				Self.Accumalate(Other);

		for(Boid& Self: Flock)
		{
			Self.Update();
			if(Self.role() == Civ) Self.Render(Civilian);
			else if(Self.role() == EMT) Self.Render(Emergency);
		}
		Civilian << endl;
		Emergency << endl;
		Render(i);
	}

	return(0);
}

void Render(int i)
{
	cout << "set terminal pngcairo size 1280,960\n";
	cout << "set output './Frame/Frame" << i << ".png'\n";
	cout << "set xrange[0:200];set yrange[0:200];set zrange[0:200]\n";
	cout << "unset border;unset xtics;unset ytics;unset ztics\n";
	cout << "set view 45,315\n";
	cout << "splot \"Frame/Civilian\" using 1:2:($3/($4>=.9)) every :::" << i << "::" << i << " title \"Boids\", \"Frame/Civilian\" using 1:2:($3/($4>.5 && $4 <=.9)) every :::" << i << "::" << i << " title \"Amblitory\", \"Frame/Civilian\" using 1:2:($3/($4>0 && $4 <=.5)) every :::" << i << "::" << i << " title \"Critical\", \"Frame/Civilian\" using 1:2:($3/($4==0)) every :::" << i << "::" << i << " title \"Dead\", \"Frame/EMT\" using 1:2:3 every :::" << i << "::" << i << " title \"EMT\"" << endl;
}
