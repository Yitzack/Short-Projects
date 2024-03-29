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
	//Filters should be ran through an appropriate LRC circuit first before trying the FFT filters
	void Low_Cut(double, double);	//uses a dB/octave roll off to cut frequencies lower than a threashold
	void Low_Cut(Track, double);
	void Low_Cut(double, Track);
	void Low_Cut(Track, Track);
	void High_Cut(double, double);	//uses a dB/octave roll off to cut frequencies higher than a threashold
	void High_Cut(Track, double);
	void High_Cut(double, Track);
	void High_Cut(Track, Track);
	void Low_Shelf(double, double, double);	//uses a dB/octave roll off to cut frequencies lower than a threashold down to a shelf (may amplify the shelf)v
	void Low_Shelf(Track, double, double);
	void Low_Shelf(double, Track, double);
	void Low_Shelf(double, double, Track);
	void Low_Shelf(double, Track, Track);
	void Low_Shelf(Track, double, Track);
	void Low_Shelf(Track, Track, double);
	void Low_Shelf(Track, Track, Track);
	void High_Shelf(double, double, double);	//uses a dB/octave roll off to cut frequencies higher than a threashold down to a shelf (may amplify the shelf)
	void High_Shelf(Track, double, double);
	void High_Shelf(double, Track, double);
	void High_Shelf(double, double, Track);
	void High_Shelf(double, Track, Track);
	void High_Shelf(Track, double, Track);
	void High_Shelf(Track, Track, double);
	void High_Shelf(Track, Track, Track);
	void Band_Pass(double, double);	//Uses a dB/octave roll off to cut frequencies above or below the named freqency which will uneffected
	void Band_Pass(Track, double);
	void Band_Pass(double, Track);
	void Band_Pass(Track, Track);
	void Band_Amp(double, double, double);	//Amplifies (or attenuates) a peak frequency in a shape around it
	void Band_Amp(Track, double, double);
	void Band_Amp(double, Track, double);
	void Band_Amp(double, double, Track);
	void Band_Amp(double, Track, Track);
	void Band_Amp(Track, double, Track);
	void Band_Amp(Track, Track, double);
	void Band_Amp(Track, Track, Track);

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
	Track operator=(Track other){ Data = other.Data; return(*this); }
	Track concat(Track);
	Track concat(double, Track);

	//Various helper functions which are useful to the user.
	int Length(){ return(Data.size()); }
	double operator[](int i){ return(Data[i]); }
	double Max();	//Returns the largest absolute value so that the user can apply amplifaction to get everything to fit right.
	Track FFT();
	Track IFFT();

	private:
	vector<double> Data;

	//Various helper functions
	double Sign(double x){ return(x==0?0:x/abs(x)); }
	int16_t Sign(int16_t x){ return(x==0?0:x/abs(x)); }
	double Sawtooth(double theta){ return(2.*(theta/(2.*M_PI)-floor(.5+theta/(2.*M_PI)))); }
	double Triangle(double theta){ return(1.-2.*abs(2.*((theta-M_PI/2.)/(2.*M_PI)-floor(.5+(theta-M_PI/2.)/(2.*M_PI))))); }
	double Vec_Max(vector<double>);	//Returns the largest absolute value so that the user can apply amplifaction to get everything to fit right.
	double Roll_off(double, double, double);
	double Shelf(double, double, double, double);
	double Peak(double, double, double, double);
	vector<double> FFT(vector<double>, const int8_t);
};

#endif
