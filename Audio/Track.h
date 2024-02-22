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
	void Frequency(float, float, float);
	void Frequency(float, Track, float);
	void Frequency(float, float, Track);
	void Frequency(float, Track, Track);
	int Length(){ return(Data.size()); }
	int16_t operator[](int i){ return(Data[i]); }
	private:
	vector<int16_t> Data;
};

#endif
