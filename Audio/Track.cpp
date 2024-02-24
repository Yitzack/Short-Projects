#include<ctime>
#include<climits>
#include"Track.h"
using namespace std;

void Track::Sine(double duration, double frequency, double amplitude, double DC)
{
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(amplitude*sin(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Sine(double duration, Track frequency, double amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(frequency.Length() < duration*SAMPLE_RATE)
		Samples = frequency.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude*sin(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Sine(double duration, double frequency, Track amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(amplitude.Length() < duration*SAMPLE_RATE)
		Samples = amplitude.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*sin(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Sine(double duration, Track frequency, Track amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(min(frequency.Length(),amplitude.Length()) < duration*SAMPLE_RATE)
		Samples = min(frequency.Length(),amplitude.Length());

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*sin(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Square(double duration, double frequency, double amplitude, double DC)
{
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(amplitude*Sign(sin(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE)))+DC);
}

void Track::Square(double duration, Track frequency, double amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(frequency.Length() < duration*SAMPLE_RATE)
		Samples = frequency.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude*Sign(sin(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE)))+DC);
}

void Track::Square(double duration, double frequency, Track amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(amplitude.Length() < duration*SAMPLE_RATE)
		Samples = amplitude.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Sign(sin(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE)))+DC);
}

void Track::Square(double duration, Track frequency, Track amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(min(frequency.Length(),amplitude.Length()) < duration*SAMPLE_RATE)
		Samples = min(frequency.Length(),amplitude.Length());

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Sign(sin(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE)))+DC);
}

void Track::Sawtooth(double duration, double frequency, double amplitude, double DC)
{
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(amplitude*Sawtooth(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Sawtooth(double duration, Track frequency, double amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(frequency.Length() < duration*SAMPLE_RATE)
		Samples = frequency.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude*Sawtooth(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Sawtooth(double duration, double frequency, Track amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(amplitude.Length() < duration*SAMPLE_RATE)
		Samples = amplitude.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Sawtooth(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Sawtooth(double duration, Track frequency, Track amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(min(frequency.Length(),amplitude.Length()) < duration*SAMPLE_RATE)
		Samples = min(frequency.Length(),amplitude.Length());

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Sawtooth(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Triangle(double duration, double frequency, double amplitude, double DC)
{
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(amplitude*Triangle(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Triangle(double duration, Track frequency, double amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(frequency.Length() < duration*SAMPLE_RATE)
		Samples = frequency.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude*Triangle(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Triangle(double duration, double frequency, Track amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(amplitude.Length() < duration*SAMPLE_RATE)
		Samples = amplitude.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Triangle(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Triangle(double duration, Track frequency, Track amplitude, double DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(min(frequency.Length(),amplitude.Length()) < duration*SAMPLE_RATE)
		Samples = min(frequency.Length(),amplitude.Length());

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Triangle(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE))+DC);
}

void Track::Noise(double duration)
{
	mt19937 RNG(time(NULL));
	uniform_real_distribution<double> Uniform(-1, 1);
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(Uniform(RNG));
}

void Track::ADSR(double start, double attack, double decay, double sustain, double sustain_level, double release)
{
	int i = start*SAMPLE_RATE;
	for(i; i < (start+attack)*SAMPLE_RATE; i++)
		Data[i] *= (i-start*SAMPLE_RATE)/(attack*SAMPLE_RATE);
	for(i; i < (start+attack+decay)*SAMPLE_RATE; i++)
		Data[i] *= (sustain_level-1.)/(decay*SAMPLE_RATE)*(i-(start+attack)*SAMPLE_RATE)+1.;
	for(i; i < (start+attack+decay+sustain)*SAMPLE_RATE; i++)
		Data[i] *= sustain_level;
	for(i; i < (start+attack+decay+sustain+decay)*SAMPLE_RATE; i++)
		Data[i] *= -sustain_level/(release*SAMPLE_RATE)*(i-(start+attack+decay+sustain)*SAMPLE_RATE)+sustain_level;
}

void Track::Compressor(double ratio, double threashold, double attack, double sustain, double release)
{
}

void Track::High_Cut(double Threashold, double Roll_off)
{
}

void Track::High_Cut(Track Threashold, double Roll_off)
{
}

void Track::High_Cut(double Threashold, Track Roll_off)
{
}

void Track::High_Cut(Track Threashold, Track Roll_off)
{
}

void Track::Low_Cut(double Threashold, double Roll_off)
{
}

void Track::Low_Cut(Track Threashold, double Roll_off)
{
}

void Track::Low_Cut(double Threashold, Track Roll_off)
{
}

void Track::Low_Cut(Track Threashold, Track Roll_off)
{
}

void Track::High_Shelf(double Threashold, double Roll_off, double Amp)
{
}

void Track::High_Shelf(Track Threashold, double Roll_off, double Amp)
{
}

void Track::High_Shelf(double Threashold, Track Roll_off, double Amp)
{
}

void Track::High_Shelf(double Threashold, double Roll_off, Track Amp)
{
}

void Track::High_Shelf(double Threashold, Track Roll_off, Track Amp)
{
}

void Track::High_Shelf(Track Threashold, double Roll_off, Track Amp)
{
}

void Track::High_Shelf(Track Threashold, Track Roll_off, double Amp)
{
}

void Track::High_Shelf(Track Threashold, Track Roll_off, Track Amp)
{
}

void Track::Low_Shelf(double Threashold, double Roll_off, double Amp)
{
}

void Track::Low_Shelf(Track Threashold, double Roll_off, double Amp)
{
}

void Track::Low_Shelf(double Threashold, Track Roll_off, double Amp)
{
}

void Track::Low_Shelf(double Threashold, double Roll_off, Track Amp)
{
}

void Track::Low_Shelf(double Threashold, Track Roll_off, Track Amp)
{
}

void Track::Low_Shelf(Track Threashold, double Roll_off, Track Amp)
{
}

void Track::Low_Shelf(Track Threashold, Track Roll_off, double Amp)
{
}

void Track::Low_Shelf(Track Threashold, Track Roll_off, Track Amp)
{
}

void Track::Band_Amp(double Frequency, double Width, double Amp)
{
}

void Track::Band_Amp(Track Frequency, double Width, double Amp)
{
}

void Track::Band_Amp(double Frequency, Track Width, double Amp)
{
}

void Track::Band_Amp(double Frequency, double Width, Track Amp)
{
}

void Track::Band_Amp(double Frequency, Track Width, Track Amp)
{
}

void Track::Band_Amp(Track Frequency, double Width, Track Amp)
{
}

void Track::Band_Amp(Track Frequency, Track Width, double Amp)
{
}

void Track::Band_Amp(Track Frequency, Track Width, Track Amp)
{
}

void Track::Band_Pass(double Frequency, double Roll_off)
{
}

void Track::Band_Pass(Track Frequency, double Roll_off)
{
}

void Track::Band_Pass(double Frequency, Track Roll_off)
{
}

void Track::Band_Pass(Track Frequency, Track Roll_off)
{
}

Track Track::Delay(double Interval, double Decay)
{
	Track Result;
	Result.Data = Data;
	for(int i = Interval*SAMPLE_RATE; i < Data.size(); i++)
		Result.Data[i] += Result.Data[i-Interval*SAMPLE_RATE]*pow(10.,Decay/10.);
	for(int i = 0; i < Data.size(); i++)
		Result.Data[i] -= Data[i];
	return(Result);
}

Track Track::Delay(Track Interval, double Decay)
{
	Track Result;
	Result.Data = Data;
	for(int i = Interval[0]*SAMPLE_RATE; i < Data.size(); i++)
		Result.Data[i] += Result.Data[i-Interval[i]*SAMPLE_RATE]*pow(10.,Decay/10.);
	for(int i = 0; i < Data.size(); i++)
		Result.Data[i] -= Data[i];
	return(Result);
}

Track Track::Delay(double Interval, Track Decay)
{
	Track Result;
	Result.Data = Data;
	for(int i = Interval*SAMPLE_RATE; i < Data.size(); i++)
		Result.Data[i] += Result.Data[i-Interval*SAMPLE_RATE]*pow(10.,Decay[i]/10.);
	for(int i = 0; i < Data.size(); i++)
		Result.Data[i] -= Data[i];
	return(Result);
}

Track Track::Delay(Track Interval, Track Decay)
{
	Track Result;
	Result.Data = Data;

	for(int i = Interval[0]*SAMPLE_RATE; i < Data.size(); i++)
		Result.Data[i] += Result.Data[i-Interval[i]*SAMPLE_RATE]*pow(10.,Decay[i]/10.);
	for(int i = 0; i < Data.size(); i++)
		Result.Data[i] -= Data[i];

	return(Result);
}

Track Track::Reverb(double source_x, double source_y, double source_z, double sink_x, double sink_y, double sink_z, double box_x, double box_y, double box_z, double attenuation)
{
	return(*this);
}

Track Track::Convolution(Track Response)
{
	Track Result;
	Result.Data.reserve(Data.size()+Response.Length());

	for(int i = Response.Length()-1; i >= 0; i--)
		for(int j = min(0,i); j < Data.size()+Response.Length(); j++)
			Result.Data[j] += Response[i]*Data[j-i];

	return(Result);
}

Track Track::Plate()
{
	return(*this);
}

Track Track::Spring()
{
	return(*this);
}

void Track::Fader(double Amp)
{
	for(int i = 0; i < Data.size(); i++)
		Data[i] *= pow(10.,Amp/10.);
}

void Track::Fader(Track Amp)
{
	for(int i = 0; i < Data.size(); i++)
		Data[i] *= pow(10.,Amp[i]/10.);
}

Track Track::operator*(double Amp)
{
	Fader(10.*log(Amp)/log(10.));
	return(*this);
}

Track Track::operator+(Track Other)
{
	return(Other);
}

Track Track::concat(Track Other)
{
	return(Other);
}

Track Track::concat(double Silence_duration, Track Other)
{
	return(Other);
}

Track Track::FFT()
{
	return(*this);
}

Track Track::IFFT()
{
	return(*this);
}

