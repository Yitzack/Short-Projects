#include<fstream>
#include<vector>
#include<random>
#include"Vector3.h"
#include"Boid.h"
using namespace std;

int main()
{
	mt19937_64 RNG;
	uniform_real_distribution<double> Uniform(0,1000);
	vector<Boid> Flock;
	ofstream Record("Frame/Frames");
	vector3 pos, vel;

	for(int i = 0; i < 100; i++)
	{
		pos = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = (vel-vector3(500.,500.,500.))/10.;
		Flock.push_back(Boid(pos, vel));
	}

	for(int i = 0; i < 10; i++)
	{
		for(Boid& Self: Flock)
			for(Boid& Other: Flock)
				Self.Accumalate(Other);
		for(Boid& Self: Flock)
		{
			Self.Update();
			Self.Render(Record);
		}
		Record << endl;
	}

	return(0);
}
