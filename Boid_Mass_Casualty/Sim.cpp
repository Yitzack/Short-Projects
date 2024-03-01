#include<iostream>
#include<fstream>
#include<vector>
#include<random>
#include"Vector3.h"
#include"Boid.h"
using namespace std;

void Render(int i);

int main()
{
	mt19937_64 RNG;
	uniform_real_distribution<double> Uniform(0,200);
	vector<Boid> Flock;
	ofstream Record("Frame/Frames");
	vector3 pos, vel;

	for(int i = 0; i < 100; i++)
	{
		pos = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = (vel-vector3(100.,100.,100.))/10.;
		Flock.push_back(Boid(pos, vel));
	}

	for(Boid& Self: Flock)
		Self.Render(Record);
	Record << endl;
	Render(0);

	for(int i = 1; i <= 100; i++)
	{
		for(Boid& Self: Flock)
			for(Boid Other: Flock)
				Self.Accumalate(Other);

		for(Boid& Self: Flock)
		{
			Self.Update();
			Self.Render(Record);
		}
		Record << endl;
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
	cout << "splot \"Frame/Frames\" using 1:2:3 every :::" << i << "::" << i << " title \"Boids\"" << endl;
}
