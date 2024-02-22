#include<vector>
#include<cmath>
#include<random>
using namespace std;

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 48000
#endif

#ifndef TRACK_H
#define TRACK_H

class Track
{
	public:
	Track(){};
	void Sine(float, float, float, float);
	void Sine(float, Track, float, float);
	void Sine(float, float, Track, float);
	void Sine(float, Track, Track, float);
	void Square(float, float, float, float);
	void Square(float, Track, float, float);
	void Square(float, float, Track, float);
	void Square(float, Track, Track, float);
	void Sawtooth(float, float, float, float);
	void Sawtooth(float, Track, float, float);
	void Sawtooth(float, float, Track, float);
	void Sawtooth(float, Track, Track, float);
	float Sawtooth(float theta){ return(2.*(theta/(2.*M_PI)-floor(.5+theta/(2.*M_PI)))); }
	void Triangle(float, float, float, float);
	void Triangle(float, Track, float, float);
	void Triangle(float, float, Track, float);
	void Triangle(float, Track, Track, float);
	float Triangle(float theta){ return(1.-2.*abs(2.*((theta-M_PI/2.)/(2.*M_PI)-floor(.5+(theta-M_PI/2.)/(2.*M_PI))))); }
	int Length(){ return(Data.size()); }
	int16_t operator[](int i){ return(Data[i]); }
	float Sign(float x){ return(x==0?0:x/abs(x)); }
	double Sign(double x){ return(x==0?0:x/abs(x)); }
	int16_t Sign(int16_t x){ return(x==0?0:x/abs(x)); }
	private:
	vector<int16_t> Data;
};

#endif
