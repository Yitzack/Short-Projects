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

