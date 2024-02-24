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
	void Sine(double, double, double, double);
	void Sine(double, Track, double, double);
	void Sine(double, double, Track, double);
	void Sine(double, Track, Track, double);
	void Square(double, double, double, double);
	void Square(double, Track, double, double);
	void Square(double, double, Track, double);
	void Square(double, Track, Track, double);
	void Sawtooth(double, double, double, double);
	void Sawtooth(double, Track, double, double);
	void Sawtooth(double, double, Track, double);
	void Sawtooth(double, Track, Track, double);
	void Triangle(double, double, double, double);
	void Triangle(double, Track, double, double);
	void Triangle(double, double, Track, double);
	void Triangle(double, Track, Track, double);
	void Noise(double);

	//Sound shaping functions
	void ADSR(double, double, double, double, double, double);
	void Compressor(double, double, double, double, double);
	void Gate();
	void Duck();
	void Low_Cut(double, double);
	void Low_Cut(Track, double);
	void Low_Cut(double, Track);
	void Low_Cut(Track, Track);
	void High_Cut(double, double);
	void High_Cut(Track, double);
	void High_Cut(double, Track);
	void High_Cut(Track, Track);
	void High_Shelf(double, double, double);
	void High_Shelf(Track, double, double);
	void High_Shelf(double, Track, double);
	void High_Shelf(double, double, Track);
	void High_Shelf(double, Track, Track);
	void High_Shelf(Track, double, Track);
	void High_Shelf(Track, Track, double);
	void High_Shelf(Track, Track, Track);
	void Low_Shelf(double, double, double);
	void Low_Shelf(Track, double, double);
	void Low_Shelf(double, Track, double);
	void Low_Shelf(double, double, Track);
	void Low_Shelf(double, Track, Track);
	void Low_Shelf(Track, double, Track);
	void Low_Shelf(Track, Track, double);
	void Low_Shelf(Track, Track, Track);
	void Band_Amp(double, double, double);
	void Band_Amp(Track, double, double);
	void Band_Amp(double, Track, double);
	void Band_Amp(double, double, Track);
	void Band_Amp(double, Track, Track);
	void Band_Amp(Track, double, Track);
	void Band_Amp(Track, Track, double);
	void Band_Amp(Track, Track, Track);
	void Band_Pass(double, double);
	void Band_Pass(Track, double);
	void Band_Pass(double, Track);
	void Band_Pass(Track, Track);

	//Effects, effect returns are dry to be mixed at user's discretion.
	Track Delay(double, double);
	Track Delay(Track, double);
	Track Delay(double, Track);
	Track Delay(Track, Track);
	Track Reverb(double, double, double, double, double, double, double, double, double, double);
	Track Convolution(Track);
	Track Plate();
	Track Spring();

	//Mixing functions
	void Fader(double);
	void Fader(Track);
	Track operator*(double);
	Track operator+(Track);
	Track concat(Track);
	Track concat(double, Track);

	//Various helper functions which are useful to the user.
	int Length(){ return(Data.size()); }
	double operator[](int i){ return(Data[i]); }
	Track FFT();
	Track IFFT();

	private:
	vector<double> Data;

	//Various helper functions
	double Sign(double x){ return(x==0?0:x/abs(x)); }
	int16_t Sign(int16_t x){ return(x==0?0:x/abs(x)); }
	double Sawtooth(double theta){ return(2.*(theta/(2.*M_PI)-floor(.5+theta/(2.*M_PI)))); }
	double Triangle(double theta){ return(1.-2.*abs(2.*((theta-M_PI/2.)/(2.*M_PI)-floor(.5+(theta-M_PI/2.)/(2.*M_PI))))); }
};

#endif
