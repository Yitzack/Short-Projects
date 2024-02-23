#include"Track.h"
using namespace std;

void Track::Sine(float duration, float frequency, float amplitude, float DC)
{
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(amplitude*sin(float(i)*frequency*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Sine(float duration, Track frequency, float amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(frequency.Length() < duration*SAMPLE_RATE)
		Samples = frequency.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude*sin(float(i)*frequency[i]*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Sine(float duration, float frequency, Track amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(amplitude.Length() < duration*SAMPLE_RATE)
		Samples = amplitude.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*sin(float(i)*frequency*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Sine(float duration, Track frequency, Track amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(min(frequency.Length(),amplitude.Length()) < duration*SAMPLE_RATE)
		Samples = min(frequency.Length(),amplitude.Length());

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*sin(float(i)*frequency[i]*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Square(float duration, float frequency, float amplitude, float DC)
{
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(amplitude*Sign(sin(float(i)*frequency*2.*M_PI/float(SAMPLE_RATE)))+DC);
}

void Track::Square(float duration, Track frequency, float amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(frequency.Length() < duration*SAMPLE_RATE)
		Samples = frequency.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude*Sign(sin(float(i)*frequency[i]*2.*M_PI/float(SAMPLE_RATE)))+DC);
}

void Track::Square(float duration, float frequency, Track amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(amplitude.Length() < duration*SAMPLE_RATE)
		Samples = amplitude.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Sign(sin(float(i)*frequency*2.*M_PI/float(SAMPLE_RATE)))+DC);
}

void Track::Square(float duration, Track frequency, Track amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(min(frequency.Length(),amplitude.Length()) < duration*SAMPLE_RATE)
		Samples = min(frequency.Length(),amplitude.Length());

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Sign(sin(float(i)*frequency[i]*2.*M_PI/float(SAMPLE_RATE)))+DC);
}

void Track::Sawtooth(float duration, float frequency, float amplitude, float DC)
{
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(amplitude*Sawtooth(float(i)*frequency*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Sawtooth(float duration, Track frequency, float amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(frequency.Length() < duration*SAMPLE_RATE)
		Samples = frequency.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude*Sawtooth(float(i)*frequency[i]*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Sawtooth(float duration, float frequency, Track amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(amplitude.Length() < duration*SAMPLE_RATE)
		Samples = amplitude.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Sawtooth(float(i)*frequency*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Sawtooth(float duration, Track frequency, Track amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(min(frequency.Length(),amplitude.Length()) < duration*SAMPLE_RATE)
		Samples = min(frequency.Length(),amplitude.Length());

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Sawtooth(float(i)*frequency[i]*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Triangle(float duration, float frequency, float amplitude, float DC)
{
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(amplitude*Triangle(float(i)*frequency*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Triangle(float duration, Track frequency, float amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(frequency.Length() < duration*SAMPLE_RATE)
		Samples = frequency.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude*Triangle(float(i)*frequency[i]*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Triangle(float duration, float frequency, Track amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(amplitude.Length() < duration*SAMPLE_RATE)
		Samples = amplitude.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Triangle(float(i)*frequency*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Triangle(float duration, Track frequency, Track amplitude, float DC)
{
	int Samples = duration*SAMPLE_RATE;
	if(min(frequency.Length(),amplitude.Length()) < duration*SAMPLE_RATE)
		Samples = min(frequency.Length(),amplitude.Length());

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*Triangle(float(i)*frequency[i]*2.*M_PI/float(SAMPLE_RATE))+DC);
}

void Track::Noise(float duration)
{
}

void Track::ADSR(float start, float attack, float decay, float sustain, float sustain_level, float release)
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

void Track::Compressor(float ratio, float threashold, float attack, float sustain, float release)
{
}

void Track::High_Cut(float Threashold, float Roll_off)
{
}

void Track::High_Cut(Track Threashold, float Roll_off)
{
}

void Track::High_Cut(float Threashold, Track Roll_off)
{
}

void Track::High_Cut(Track Threashold, Track Roll_off)
{
}

void Track::Low_Cut(float Threashold, float Roll_off)
{
}

void Track::Low_Cut(Track Threashold, float Roll_off)
{
}

void Track::Low_Cut(float Threashold, Track Roll_off)
{
}

void Track::Low_Cut(Track Threashold, Track Roll_off)
{
}

void Track::High_Shelf(float Threashold, float Roll_off, float Amp)
{
}

void Track::High_Shelf(Track Threashold, float Roll_off, float Amp)
{
}

void Track::High_Shelf(float Threashold, Track Roll_off, float Amp)
{
}

void Track::High_Shelf(float Threashold, float Roll_off, Track Amp)
{
}

void Track::High_Shelf(float Threashold, Track Roll_off, Track Amp)
{
}

void Track::High_Shelf(Track Threashold, float Roll_off, Track Amp)
{
}

void Track::High_Shelf(Track Threashold, Track Roll_off, float Amp)
{
}

void Track::High_Shelf(Track Threashold, Track Roll_off, Track Amp)
{
}

void Track::Low_Shelf(float Threashold, float Roll_off, float Amp)
{
}

void Track::Low_Shelf(Track Threashold, float Roll_off, float Amp)
{
}

void Track::Low_Shelf(float Threashold, Track Roll_off, float Amp)
{
}

void Track::Low_Shelf(float Threashold, float Roll_off, Track Amp)
{
}

void Track::Low_Shelf(float Threashold, Track Roll_off, Track Amp)
{
}

void Track::Low_Shelf(Track Threashold, float Roll_off, Track Amp)
{
}

void Track::Low_Shelf(Track Threashold, Track Roll_off, float Amp)
{
}

void Track::Low_Shelf(Track Threashold, Track Roll_off, Track Amp)
{
}

void Track::Band_Amp(float Frequency, float Width, float Amp)
{
}

void Track::Band_Amp(Track Frequency, float Width, float Amp)
{
}

void Track::Band_Amp(float Frequency, Track Width, float Amp)
{
}

void Track::Band_Amp(float Frequency, float Width, Track Amp)
{
}

void Track::Band_Amp(float Frequency, Track Width, Track Amp)
{
}

void Track::Band_Amp(Track Frequency, float Width, Track Amp)
{
}

void Track::Band_Amp(Track Frequency, Track Width, float Amp)
{
}

void Track::Band_Amp(Track Frequency, Track Width, Track Amp)
{
}

void Track::Band_Pass(float Frequency, float Roll_off)
{
}

void Track::Band_Pass(Track Frequency, float Roll_off)
{
}

void Track::Band_Pass(float Frequency, Track Roll_off)
{
}

void Track::Band_Pass(Track Frequency, Track Roll_off)
{
}

Track Track::Delay(float Interval, float Decay)
{
	Track Result;
	Result.Data = Data;
	for(int i = Interval*SAMPLE_RATE; i < Data.size(); i++)
		Result.Data[i] += Result.Data[i-Interval*SAMPLE_RATE]*pow(10.,Decay/10.);
	for(int i = 0; i < Data.size(); i++)
		Result.Data[i] -= Data[i];
	return(Result);
}

Track Track::Delay(Track Interval, float Decay)
{
	Track Result;
	Result.Data = Data;
	for(int i = Interval[0]*SAMPLE_RATE; i < Data.size(); i++)
		Result.Data[i] += Result.Data[i-Interval[i]*SAMPLE_RATE]*pow(10.,Decay/10.);
	for(int i = 0; i < Data.size(); i++)
		Result.Data[i] -= Data[i];
	return(Result);
}

Track Track::Delay(float Interval, Track Decay)
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

Track Track::Reverb(float source_x, float source_y, float source_z, float sink_x, float sink_y, float sink_z, float box_x, float box_y, float box_z, float attenuation)
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

void Track::Fader(float Amp)
{
	for(int i = 0; i < Data.size(); i++)
	{
		if(abs(((1<<16)-1)/Data[i]) < pow(10.,Amp/10.))
			Data[i] = ((1<<16)-1)*Sign(Data[i]);
		else
			Data[i] *= pow(10.,Amp/10.);
	}
}

void Track::Fader(Track Amp)
{
	for(int i = 0; i < Data.size(); i++)
	{
		if(abs(((1<<16)-1)/Data[i]) < pow(10.,Amp[i]/10.))
			Data[i] = ((1<<16)-1)*Sign(Data[i]);
		else
			Data[i] *= pow(10.,Amp[i]/10.);
	}
}

Track Track::operator*(float Amp)
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

Track Track::concat(float Silence_duration, Track Other)
{
	return(Other);
}

