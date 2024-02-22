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

void Track::High_Shelf(float Threashold, float Roll_off, Track Amp)
{
}

void Track::High_Shelf(Track Threashold, float Roll_off, float Amp)
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

void Track::Low_Shelf(float Threashold, float Roll_off, Track Amp)
{
}

void Track::Low_Shelf(Track Threashold, float Roll_off, float Amp)
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

void Track::Band_Amp(float Frequency, float Width, Track Amp)
{
}

void Track::Band_Amp(Track Frequency, float Width, float Amp)
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

void Track::Fader(float Amp)
{
}

void Track::Fader(Track Amp)
{
}

Track Track::operator*(float Amp)
{
}

Track Track::operator+(Track Other)
{
}

Track Track::concat(Track Other)
{
}

Track Track::concat(float Silence_duration, Track Other)
{
}

