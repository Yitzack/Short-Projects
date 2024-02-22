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

	//Simple function generators and FM synthesis
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
	void Noise(float);

	//Sound shaping functions
	void ADSR(float, float, float, float, float, float);
	void Compressor(float, float, float, float, float);
	void Gate();
	void Duck();
	void Low_Cut(float, float);
	void Low_Cut(Track, float);
	void Low_Cut(float, Track);
	void Low_Cut(Track, Track);
	void High_cut(float, float);
	void High_cut(Track, float);
	void High_cut(float, Track);
	void High_cut(Track, Track);
	void High_Shelf(float, float, float);
	void High_Shelf(Track, float, float);
	void High_Shelf(float, Track, float);
	void High_Shelf(float, float, Track);
	void High_Shelf(float, Track, Track);
	void High_Shelf(float, float, Track);
	void High_Shelf(Track, float, float);
	void High_Shelf(Track, Track, float);
	void High_Shelf(Track, Track, Track);
	void Low_Shelf(float, float, float);
	void Low_Shelf(Track, float, float);
	void Low_Shelf(float, Track, float);
	void Low_Shelf(float, float, Track);
	void Low_Shelf(float, Track, Track);
	void Low_Shelf(float, float, Track);
	void Low_Shelf(Track, float, float);
	void Low_Shelf(Track, Track, float);
	void Low_Shelf(Track, Track, Track);
	void Band_Amp(float, float, float);
	void Band_Amp(Track, float, float);
	void Band_Amp(float, Track, float);
	void Band_Amp(float, float, Track);
	void Band_Amp(float, Track, Track);
	void Band_Amp(float, float, Track);
	void Band_Amp(Track, float, float);
	void Band_Amp(Track, Track, float);
	void Band_Amp(Track, Track, Track);
	void Band_Pass(float, float);
	void Band_Pass(Track, float);
	void Band_Pass(float, Track);
	void Band_Pass(Track, Track);

	//Effects
	void Delay(float, float);
	void Delay(Track, float);
	void Delay(float, Track);
	void Delay(Track, Track);
	void Reverb(float, float, float, float, float, float, float, float, float);
	void Plate();
	void Spring();

	//Mixing functions
	void Fader(float);
	void Fader(Track);
	Track operator*(float);
	Track operator+(Track);
	Track concat(Track);
	Track concat(float, Track);

	//Various helper functions
	int Length(){ return(Data.size()); }
	int16_t operator[](int i){ return(Data[i]); }
	float Sign(float x){ return(x==0?0:x/abs(x)); }
	double Sign(double x){ return(x==0?0:x/abs(x)); }
	int16_t Sign(int16_t x){ return(x==0?0:x/abs(x)); }

	private:
	vector<int16_t> Data;
};

#endif
