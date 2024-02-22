#include"Track.h"
using namespace std;

void Track::Frequency(float duration, float frequency, float amplitude)
{
	for(int i = 0; i < duration*SAMPLE_RATE; i++)
		Data.push_back(amplitude*sin(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE))*((1 << 15)-1));
}

void Track::Frequency(float duration, Track frequency, float amplitude)
{
	int Samples = duration*SAMPLE_RATE;
	if(frequency.Length() < duration*SAMPLE_RATE)
		Samples = frequency.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude*sin(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE))*((1 << 15)-1));
}

void Track::Frequency(float duration, float frequency, Track amplitude)
{
	int Samples = duration*SAMPLE_RATE;
	if(amplitude.Length() < duration*SAMPLE_RATE)
		Samples = amplitude.Length();

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*sin(double(i)*frequency*2.*M_PI/double(SAMPLE_RATE))*((1 << 15)-1));
}

void Track::Frequency(float duration, Track frequency, Track amplitude)
{
	int Samples = duration*SAMPLE_RATE;
	if(min(frequency.Length(),amplitude.Length()) < duration*SAMPLE_RATE)
		Samples = min(frequency.Length(),amplitude.Length());

	for(int i = 0; i < Samples; i++)
		Data.push_back(amplitude[i]*sin(double(i)*frequency[i]*2.*M_PI/double(SAMPLE_RATE))*((1 << 15)-1));
}

