#include<ctime>
#include<climits>
#include<algorithm>
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

double Roll_off(double frequency, double threashold, double rolloff)
{
	return(0);
}

double Shelf(double frequency, double threashold, double rolloff, double shelf)	//This one might become Roll_off with shelf of -inf dB
{
	return(0);
}

double Peak(double frequency, double center, double width, double amplification)
{
	return(0);
}

void Track::High_Cut(double Threashold, double Roll_off)
{
	double RC = 1./(2.*M_PI*Threashold);	//Threashold frequency is the frequency where there 3dB attenuation in the signal
	vector<double> Result;
	double Q[3];

	for(int i = 0; i < Data.size(); i++)
	{
		if(i != 0)
			Q[0] = pow(SAMPLE_RATE,2)*(Data[i-1]*(pow(RC,2)+2.*RC/SAMPLE_RATE+2./pow(SAMPLE_RATE,2))-Data[i]*(2.*pow(RC,2)+2.*RC/SAMPLE_RATE)+Data[i+1]*pow(RC,2))/2.;		
		Q[1] = pow(SAMPLE_RATE,2)*(Data[i-1]*(pow(RC,2)+RC/SAMPLE_RATE)+2.*Data[i]/pow(SAMPLE_RATE,2)+Data[i+1]*(pow(RC,2)-RC/SAMPLE_RATE))/2.;
		if(i+1 != Data.size())
			Q[2] = pow(SAMPLE_RATE,2)*(Data[i+1]*(pow(RC,2)-2.*RC/SAMPLE_RATE+2./pow(SAMPLE_RATE,2))+Data[i]*(2.*RC/SAMPLE_RATE-2.*pow(RC,2))+Data[i-1]*pow(RC,2))/2.;

		if(i != 0 && i+1 != Data.size())
			Result.push_back((Q[0]+Q[1]+Q[2])/3.);
		else if(i == 0)
			Result.push_back((Q[1]+Q[2])/2.);
		else
			Result.push_back((Q[0]+Q[1])/2.);
	}

	double Max = Vec_Max(Result);
	for(int i = 0; i < Result.size(); i++)
	{
		Result[i] = Data[i]-Result[i]/Max;	//Low pass filter results + High pass filter results = Unity (no change; easier than redoing the math to find RI)
	}

	Data = Result;
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
	double RC = 1./(2.*M_PI*Threashold);	//Threashold frequency is the frequency where there 3dB attenuation in the signal
	vector<double> Result;
	double Q[3];

	for(int i = 0; i < Data.size(); i++)
	{
		if(i != 0)
			Q[0] = pow(SAMPLE_RATE,2)*(Data[i-1]*(pow(RC,2)+2.*RC/SAMPLE_RATE+2./pow(SAMPLE_RATE,2))-Data[i]*(2.*pow(RC,2)+2.*RC/SAMPLE_RATE)+Data[i+1]*pow(RC,2))/2.;		
		Q[1] = pow(SAMPLE_RATE,2)*(Data[i-1]*(pow(RC,2)+RC/SAMPLE_RATE)+2.*Data[i]/pow(SAMPLE_RATE,2)+Data[i+1]*(pow(RC,2)-RC/SAMPLE_RATE))/2.;
		if(i+1 != Data.size())
			Q[2] = pow(SAMPLE_RATE,2)*(Data[i+1]*(pow(RC,2)-2.*RC/SAMPLE_RATE+2./pow(SAMPLE_RATE,2))+Data[i]*(2.*RC/SAMPLE_RATE-2.*pow(RC,2))+Data[i-1]*pow(RC,2))/2.;

		if(i != 0 && i+1 != Data.size())
			Result.push_back((Q[0]+Q[1]+Q[2])/3.);
		else if(i == 0)
			Result.push_back((Q[1]+Q[2])/2.);
		else
			Result.push_back((Q[0]+Q[1])/2.);
	}

	double Max = Vec_Max(Result);
	for(int i = 0; i < Result.size(); i++)
	{
		Result[i] /= Max;
	}

	Data = Result;
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
	double Source_Loc[int((1024./box_x+1)*(1024./box_y+1)*(1024./box_z+1))][3];
	int i, j, k, l;
	int count;

	for(count = 0; count < 8; count++)	//Get the positions around the origin
	{
		Source_Loc[count][0] = pow(-1,count & 1)*source_x;
		Source_Loc[count][1] = pow(-1,count & 2)*source_y;
		Source_Loc[count][2] = pow(-1,count & 4)*source_z;
	}

	for(i = -256./box_x; i <= 256./box_x; i++)
	{
		for(j = -256./box_y; j <= 256./box_y; j++)
		{
			for(k = -256./box_x; k <= 256./box_x; k++)
			{
				for(l = 0; l < 8; l++)
				{
					if(i != 0 && j != 0 && k != 0)
					{
						Source_Loc[count][0] = Source_Loc[l][0] + 2.*box_x*i;
						Source_Loc[count][1] = Source_Loc[l][1] + 2.*box_y*j;
						Source_Loc[count][2] = Source_Loc[l][2] + 2.*box_z*k;
						count++;
					}
				}
			}
		}
	}

	pair<double, double> Delay_Atten[count];
	for(i = 0; i < count; i++)
	{
		double distance = sqrt(pow(Source_Loc[i][0]-sink_x,2)+pow(Source_Loc[i][1]-sink_y,2)+pow(Source_Loc[i][2]-sink_z,2));
		if(distance < 256.)	//Farther than 256 meters, takes a 0dB signal to below -90.3 dB or the 1st level of sound
		{
			Delay_Atten[i].first = 340./distance;	//Delay time
			Delay_Atten[i].second = 1./pow(distance,2);	//Attenuation over distance
			Delay_Atten[i].second *= pow(attenuation,int((Source_Loc[i][0]-sink_x)/box_x));	//Number of walls passed through in x
			Delay_Atten[i].second *= pow(attenuation,int((Source_Loc[i][1]-sink_y)/box_y));	//Number of walls passed through in y
			Delay_Atten[i].second *= pow(attenuation,int((Source_Loc[i][2]-sink_z)/box_z));	//Number of walls passed through in z
		}

		if(distance >= 256. || Delay_Atten[i].second < pow(256.,-2))
		{
			Delay_Atten[i].first = 0;
			Delay_Atten[i].second = 0;
		}
	}

	sort(&Delay_Atten[0], &Delay_Atten[count-1]);

	Track Impulse_Response;
	Impulse_Response.Data.resize(int(Delay_Atten[count-1].first*SAMPLE_RATE),0);
	for(i = 0; i < count; i++)
	{
		if(Delay_Atten[i].first != 0)
		{
			Impulse_Response.Data[int(Delay_Atten[i].first*SAMPLE_RATE)] = Delay_Atten[i].second;
		}
	}

	return(Convolution(Impulse_Response));
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
		Data[i] *= pow(10.,Amp/20.);
}

void Track::Fader(Track Amp)
{
	for(int i = 0; i < Data.size(); i++)
		Data[i] *= pow(10.,Amp[i]/20.);
}

Track Track::operator*(double Amp)
{
	Fader(20.*log(Amp)/log(10.));
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
	int i, j;
	vector<double> Packed(8192);
	Track Fourier;

	for(i = 0; i < Data.size()/8192; i++)	//While there exists N*8192 samples
	{
		for(j = 0; j < 4096; j++)
		{
			Packed[2*j] = Data[8192*i+j];
			Packed[2*j+1] = Data[8192*i+j+4096];
		}
		Packed = FFT(Packed, 1);
		for(double element: Packed)
			Fourier.Data.push_back(element);
	}

	if(i*8192 != Data.size())	//There aren't enough samples to fill a full set.
	{
		for(j = 0; j < 4096; j++)
		{
			if(8192*i+j < Data.size())
				Packed[2*j] = Data[8192*i+j];
			else
				Packed[2*j] = 0;

			if(8192*i+j+4096 < Data.size())
				Packed[2*j+1] = Data[8192*i+j+4096];
			else
				Packed[2*j+1] = 0;
		}
		Packed = FFT(Packed, 1);
		for(double element: Packed)
			Fourier.Data.push_back(element);
	}

	return(Fourier);
}

Track Track::IFFT()
{
	int i, j;
	vector<double> Packed(8192);
	Track Fourier;

	for(i = 0; i < Data.size()/8192; i++)	//While there exists N*8192 samples
	{
		for(j = 0; j < 4096; j++)
		{
			Packed[2*j] = Data[8192*i+j];
			Packed[2*j+1] = Data[8192*i+j+4096];
		}
		Packed = FFT(Packed, -1);
		for(double element: Packed)
			Fourier.Data.push_back(element/4096.);
	}

	if(i*8192 != Data.size())	//There aren't enough samples to fill a full set.
	{
		for(j = 0; j < 4096; j++)
		{
			if(8192*i+j < Data.size())
				Packed[2*j] = Data[8192*i+j];
			else
				Packed[2*j] = 0;

			if(8192*i+j+4096 < Data.size())
				Packed[2*j+1] = Data[8192*i+j+4096];
			else
				Packed[2*j+1] = 0;
		}
		Packed = FFT(Packed, -1);
		for(double element: Packed)
			Fourier.Data.push_back(element/4096.);
	}

	while(Fourier.Data.back() == 0)
		Fourier.Data.pop_back();

	return(Fourier);
}

vector<double> Track::FFT(vector<double> data, const int8_t isign)
{
	int nn, mmax, m, i, j, istep;
	double wtemp, wr, wpr, wpi, wi, theta, tempr, tempi;

	int exchange[] = {0,2048,1024,3072,512,2560,1536,3584,256,2304,1280,3328,768,2816,1792,3840,128,2176,1152,3200,640,2688,1664,3712,384,2432,1408,3456,896,2944,1920,
3968,64,2112,1088,3136,576,2624,1600,3648,320,2368,1344,3392,832,2880,1856,3904,192,2240,1216,3264,704,2752,1728,3776,448,2496,1472,3520,960,3008,1984,4032,32,2080,1056,
3104,544,2592,1568,3616,288,2336,1312,3360,800,2848,1824,3872,160,2208,1184,3232,672,2720,1696,3744,416,2464,1440,3488,928,2976,1952,4000,96,2144,1120,3168,608,2656,1632,
3680,352,2400,1376,3424,864,2912,1888,3936,224,2272,1248,3296,736,2784,1760,3808,480,2528,1504,3552,992,3040,2016,4064,16,2064,1040,3088,528,2576,1552,3600,272,2320,1296,
3344,784,2832,1808,3856,144,2192,1168,3216,656,2704,1680,3728,400,2448,1424,3472,912,2960,1936,3984,80,2128,1104,3152,592,2640,1616,3664,336,2384,1360,3408,848,2896,1872,
3920,208,2256,1232,3280,720,2768,1744,3792,464,2512,1488,3536,976,3024,2000,4048,48,2096,1072,3120,560,2608,1584,3632,304,2352,1328,3376,816,2864,1840,3888,176,2224,1200,
3248,688,2736,1712,3760,432,2480,1456,3504,944,2992,1968,4016,112,2160,1136,3184,624,2672,1648,3696,368,2416,1392,3440,880,2928,1904,3952,240,2288,1264,3312,752,2800,1776,
3824,496,2544,1520,3568,1008,3056,2032,4080,8,2056,1032,3080,520,2568,1544,3592,264,2312,1288,3336,776,2824,1800,3848,136,2184,1160,3208,648,2696,1672,3720,392,2440,1416,
3464,904,2952,1928,3976,72,2120,1096,3144,584,2632,1608,3656,328,2376,1352,3400,840,2888,1864,3912,200,2248,1224,3272,712,2760,1736,3784,456,2504,1480,3528,968,3016,1992,
4040,40,2088,1064,3112,552,2600,1576,3624,296,2344,1320,3368,808,2856,1832,3880,168,2216,1192,3240,680,2728,1704,3752,424,2472,1448,3496,936,2984,1960,4008,104,2152,1128,
3176,616,2664,1640,3688,360,2408,1384,3432,872,2920,1896,3944,232,2280,1256,3304,744,2792,1768,3816,488,2536,1512,3560,1000,3048,2024,4072,24,2072,1048,3096,536,2584,1560,
3608,280,2328,1304,3352,792,2840,1816,3864,152,2200,1176,3224,664,2712,1688,3736,408,2456,1432,3480,920,2968,1944,3992,88,2136,1112,3160,600,2648,1624,3672,344,2392,1368,
3416,856,2904,1880,3928,216,2264,1240,3288,728,2776,1752,3800,472,2520,1496,3544,984,3032,2008,4056,56,2104,1080,3128,568,2616,1592,3640,312,2360,1336,3384,824,2872,1848,
3896,184,2232,1208,3256,696,2744,1720,3768,440,2488,1464,3512,952,3000,1976,4024,120,2168,1144,3192,632,2680,1656,3704,376,2424,1400,3448,888,2936,1912,3960,248,2296,1272,
3320,760,2808,1784,3832,504,2552,1528,3576,1016,3064,2040,4088,4,2052,1028,3076,516,2564,1540,3588,260,2308,1284,3332,772,2820,1796,3844,132,2180,1156,3204,644,2692,1668,
3716,388,2436,1412,3460,900,2948,1924,3972,68,2116,1092,3140,580,2628,1604,3652,324,2372,1348,3396,836,2884,1860,3908,196,2244,1220,3268,708,2756,1732,3780,452,2500,1476,
3524,964,3012,1988,4036,36,2084,1060,3108,548,2596,1572,3620,292,2340,1316,3364,804,2852,1828,3876,164,2212,1188,3236,676,2724,1700,3748,420,2468,1444,3492,932,2980,1956,
4004,100,2148,1124,3172,612,2660,1636,3684,356,2404,1380,3428,868,2916,1892,3940,228,2276,1252,3300,740,2788,1764,3812,484,2532,1508,3556,996,3044,2020,4068,20,2068,1044,
3092,532,2580,1556,3604,276,2324,1300,3348,788,2836,1812,3860,148,2196,1172,3220,660,2708,1684,3732,404,2452,1428,3476,916,2964,1940,3988,84,2132,1108,3156,596,2644,1620,
3668,340,2388,1364,3412,852,2900,1876,3924,212,2260,1236,3284,724,2772,1748,3796,468,2516,1492,3540,980,3028,2004,4052,52,2100,1076,3124,564,2612,1588,3636,308,2356,1332,
3380,820,2868,1844,3892,180,2228,1204,3252,692,2740,1716,3764,436,2484,1460,3508,948,2996,1972,4020,116,2164,1140,3188,628,2676,1652,3700,372,2420,1396,3444,884,2932,1908,
3956,244,2292,1268,3316,756,2804,1780,3828,500,2548,1524,3572,1012,3060,2036,4084,12,2060,1036,3084,524,2572,1548,3596,268,2316,1292,3340,780,2828,1804,3852,140,2188,1164,
3212,652,2700,1676,3724,396,2444,1420,3468,908,2956,1932,3980,76,2124,1100,3148,588,2636,1612,3660,332,2380,1356,3404,844,2892,1868,3916,204,2252,1228,3276,716,2764,1740,
3788,460,2508,1484,3532,972,3020,1996,4044,44,2092,1068,3116,556,2604,1580,3628,300,2348,1324,3372,812,2860,1836,3884,172,2220,1196,3244,684,2732,1708,3756,428,2476,1452,
3500,940,2988,1964,4012,108,2156,1132,3180,620,2668,1644,3692,364,2412,1388,3436,876,2924,1900,3948,236,2284,1260,3308,748,2796,1772,3820,492,2540,1516,3564,1004,3052,2028,
4076,28,2076,1052,3100,540,2588,1564,3612,284,2332,1308,3356,796,2844,1820,3868,156,2204,1180,3228,668,2716,1692,3740,412,2460,1436,3484,924,2972,1948,3996,92,2140,1116,
3164,604,2652,1628,3676,348,2396,1372,3420,860,2908,1884,3932,220,2268,1244,3292,732,2780,1756,3804,476,2524,1500,3548,988,3036,2012,4060,60,2108,1084,3132,572,2620,1596,
3644,316,2364,1340,3388,828,2876,1852,3900,188,2236,1212,3260,700,2748,1724,3772,444,2492,1468,3516,956,3004,1980,4028,124,2172,1148,3196,636,2684,1660,3708,380,2428,1404,
3452,892,2940,1916,3964,252,2300,1276,3324,764,2812,1788,3836,508,2556,1532,3580,1020,3068,2044,4092,2,2050,1026,3074,514,2562,1538,3586,258,2306,1282,3330,770,2818,1794,
3842,130,2178,1154,3202,642,2690,1666,3714,386,2434,1410,3458,898,2946,1922,3970,66,2114,1090,3138,578,2626,1602,3650,322,2370,1346,3394,834,2882,1858,3906,194,2242,1218,
3266,706,2754,1730,3778,450,2498,1474,3522,962,3010,1986,4034,34,2082,1058,3106,546,2594,1570,3618,290,2338,1314,3362,802,2850,1826,3874,162,2210,1186,3234,674,2722,1698,
3746,418,2466,1442,3490,930,2978,1954,4002,98,2146,1122,3170,610,2658,1634,3682,354,2402,1378,3426,866,2914,1890,3938,226,2274,1250,3298,738,2786,1762,3810,482,2530,1506,
3554,994,3042,2018,4066,18,2066,1042,3090,530,2578,1554,3602,274,2322,1298,3346,786,2834,1810,3858,146,2194,1170,3218,658,2706,1682,3730,402,2450,1426,3474,914,2962,1938,
3986,82,2130,1106,3154,594,2642,1618,3666,338,2386,1362,3410,850,2898,1874,3922,210,2258,1234,3282,722,2770,1746,3794,466,2514,1490,3538,978,3026,2002,4050,50,2098,1074,
3122,562,2610,1586,3634,306,2354,1330,3378,818,2866,1842,3890,178,2226,1202,3250,690,2738,1714,3762,434,2482,1458,3506,946,2994,1970,4018,114,2162,1138,3186,626,2674,1650,
3698,370,2418,1394,3442,882,2930,1906,3954,242,2290,1266,3314,754,2802,1778,3826,498,2546,1522,3570,1010,3058,2034,4082,10,2058,1034,3082,522,2570,1546,3594,266,2314,1290,
3338,778,2826,1802,3850,138,2186,1162,3210,650,2698,1674,3722,394,2442,1418,3466,906,2954,1930,3978,74,2122,1098,3146,586,2634,1610,3658,330,2378,1354,3402,842,2890,1866,
3914,202,2250,1226,3274,714,2762,1738,3786,458,2506,1482,3530,970,3018,1994,4042,42,2090,1066,3114,554,2602,1578,3626,298,2346,1322,3370,810,2858,1834,3882,170,2218,1194,
3242,682,2730,1706,3754,426,2474,1450,3498,938,2986,1962,4010,106,2154,1130,3178,618,2666,1642,3690,362,2410,1386,3434,874,2922,1898,3946,234,2282,1258,3306,746,2794,1770,
3818,490,2538,1514,3562,1002,3050,2026,4074,26,2074,1050,3098,538,2586,1562,3610,282,2330,1306,3354,794,2842,1818,3866,154,2202,1178,3226,666,2714,1690,3738,410,2458,1434,
3482,922,2970,1946,3994,90,2138,1114,3162,602,2650,1626,3674,346,2394,1370,3418,858,2906,1882,3930,218,2266,1242,3290,730,2778,1754,3802,474,2522,1498,3546,986,3034,2010,
4058,58,2106,1082,3130,570,2618,1594,3642,314,2362,1338,3386,826,2874,1850,3898,186,2234,1210,3258,698,2746,1722,3770,442,2490,1466,3514,954,3002,1978,4026,122,2170,1146,
3194,634,2682,1658,3706,378,2426,1402,3450,890,2938,1914,3962,250,2298,1274,3322,762,2810,1786,3834,506,2554,1530,3578,1018,3066,2042,4090,6,2054,1030,3078,518,2566,1542,
3590,262,2310,1286,3334,774,2822,1798,3846,134,2182,1158,3206,646,2694,1670,3718,390,2438,1414,3462,902,2950,1926,3974,70,2118,1094,3142,582,2630,1606,3654,326,2374,1350,
3398,838,2886,1862,3910,198,2246,1222,3270,710,2758,1734,3782,454,2502,1478,3526,966,3014,1990,4038,38,2086,1062,3110,550,2598,1574,3622,294,2342,1318,3366,806,2854,1830,
3878,166,2214,1190,3238,678,2726,1702,3750,422,2470,1446,3494,934,2982,1958,4006,102,2150,1126,3174,614,2662,1638,3686,358,2406,1382,3430,870,2918,1894,3942,230,2278,1254,
3302,742,2790,1766,3814,486,2534,1510,3558,998,3046,2022,4070,22,2070,1046,3094,534,2582,1558,3606,278,2326,1302,3350,790,2838,1814,3862,150,2198,1174,3222,662,2710,1686,
3734,406,2454,1430,3478,918,2966,1942,3990,86,2134,1110,3158,598,2646,1622,3670,342,2390,1366,3414,854,2902,1878,3926,214,2262,1238,3286,726,2774,1750,3798,470,2518,1494,
3542,982,3030,2006,4054,54,2102,1078,3126,566,2614,1590,3638,310,2358,1334,3382,822,2870,1846,3894,182,2230,1206,3254,694,2742,1718,3766,438,2486,1462,3510,950,2998,1974,
4022,118,2166,1142,3190,630,2678,1654,3702,374,2422,1398,3446,886,2934,1910,3958,246,2294,1270,3318,758,2806,1782,3830,502,2550,1526,3574,1014,3062,2038,4086,14,2062,1038,
3086,526,2574,1550,3598,270,2318,1294,3342,782,2830,1806,3854,142,2190,1166,3214,654,2702,1678,3726,398,2446,1422,3470,910,2958,1934,3982,78,2126,1102,3150,590,2638,1614,
3662,334,2382,1358,3406,846,2894,1870,3918,206,2254,1230,3278,718,2766,1742,3790,462,2510,1486,3534,974,3022,1998,4046,46,2094,1070,3118,558,2606,1582,3630,302,2350,1326,
3374,814,2862,1838,3886,174,2222,1198,3246,686,2734,1710,3758,430,2478,1454,3502,942,2990,1966,4014,110,2158,1134,3182,622,2670,1646,3694,366,2414,1390,3438,878,2926,1902,
3950,238,2286,1262,3310,750,2798,1774,3822,494,2542,1518,3566,1006,3054,2030,4078,30,2078,1054,3102,542,2590,1566,3614,286,2334,1310,3358,798,2846,1822,3870,158,2206,1182,
3230,670,2718,1694,3742,414,2462,1438,3486,926,2974,1950,3998,94,2142,1118,3166,606,2654,1630,3678,350,2398,1374,3422,862,2910,1886,3934,222,2270,1246,3294,734,2782,1758,
3806,478,2526,1502,3550,990,3038,2014,4062,62,2110,1086,3134,574,2622,1598,3646,318,2366,1342,3390,830,2878,1854,3902,190,2238,1214,3262,702,2750,1726,3774,446,2494,1470,
3518,958,3006,1982,4030,126,2174,1150,3198,638,2686,1662,3710,382,2430,1406,3454,894,2942,1918,3966,254,2302,1278,3326,766,2814,1790,3838,510,2558,1534,3582,1022,3070,2046,
4094,1,2049,1025,3073,513,2561,1537,3585,257,2305,1281,3329,769,2817,1793,3841,129,2177,1153,3201,641,2689,1665,3713,385,2433,1409,3457,897,2945,1921,3969,65,2113,1089,
3137,577,2625,1601,3649,321,2369,1345,3393,833,2881,1857,3905,193,2241,1217,3265,705,2753,1729,3777,449,2497,1473,3521,961,3009,1985,4033,33,2081,1057,3105,545,2593,1569,
3617,289,2337,1313,3361,801,2849,1825,3873,161,2209,1185,3233,673,2721,1697,3745,417,2465,1441,3489,929,2977,1953,4001,97,2145,1121,3169,609,2657,1633,3681,353,2401,1377,
3425,865,2913,1889,3937,225,2273,1249,3297,737,2785,1761,3809,481,2529,1505,3553,993,3041,2017,4065,17,2065,1041,3089,529,2577,1553,3601,273,2321,1297,3345,785,2833,1809,
3857,145,2193,1169,3217,657,2705,1681,3729,401,2449,1425,3473,913,2961,1937,3985,81,2129,1105,3153,593,2641,1617,3665,337,2385,1361,3409,849,2897,1873,3921,209,2257,1233,
3281,721,2769,1745,3793,465,2513,1489,3537,977,3025,2001,4049,49,2097,1073,3121,561,2609,1585,3633,305,2353,1329,3377,817,2865,1841,3889,177,2225,1201,3249,689,2737,1713,
3761,433,2481,1457,3505,945,2993,1969,4017,113,2161,1137,3185,625,2673,1649,3697,369,2417,1393,3441,881,2929,1905,3953,241,2289,1265,3313,753,2801,1777,3825,497,2545,1521,
3569,1009,3057,2033,4081,9,2057,1033,3081,521,2569,1545,3593,265,2313,1289,3337,777,2825,1801,3849,137,2185,1161,3209,649,2697,1673,3721,393,2441,1417,3465,905,2953,1929,
3977,73,2121,1097,3145,585,2633,1609,3657,329,2377,1353,3401,841,2889,1865,3913,201,2249,1225,3273,713,2761,1737,3785,457,2505,1481,3529,969,3017,1993,4041,41,2089,1065,
3113,553,2601,1577,3625,297,2345,1321,3369,809,2857,1833,3881,169,2217,1193,3241,681,2729,1705,3753,425,2473,1449,3497,937,2985,1961,4009,105,2153,1129,3177,617,2665,1641,
3689,361,2409,1385,3433,873,2921,1897,3945,233,2281,1257,3305,745,2793,1769,3817,489,2537,1513,3561,1001,3049,2025,4073,25,2073,1049,3097,537,2585,1561,3609,281,2329,1305,
3353,793,2841,1817,3865,153,2201,1177,3225,665,2713,1689,3737,409,2457,1433,3481,921,2969,1945,3993,89,2137,1113,3161,601,2649,1625,3673,345,2393,1369,3417,857,2905,1881,
3929,217,2265,1241,3289,729,2777,1753,3801,473,2521,1497,3545,985,3033,2009,4057,57,2105,1081,3129,569,2617,1593,3641,313,2361,1337,3385,825,2873,1849,3897,185,2233,1209,
3257,697,2745,1721,3769,441,2489,1465,3513,953,3001,1977,4025,121,2169,1145,3193,633,2681,1657,3705,377,2425,1401,3449,889,2937,1913,3961,249,2297,1273,3321,761,2809,1785,
3833,505,2553,1529,3577,1017,3065,2041,4089,5,2053,1029,3077,517,2565,1541,3589,261,2309,1285,3333,773,2821,1797,3845,133,2181,1157,3205,645,2693,1669,3717,389,2437,1413,
3461,901,2949,1925,3973,69,2117,1093,3141,581,2629,1605,3653,325,2373,1349,3397,837,2885,1861,3909,197,2245,1221,3269,709,2757,1733,3781,453,2501,1477,3525,965,3013,1989,
4037,37,2085,1061,3109,549,2597,1573,3621,293,2341,1317,3365,805,2853,1829,3877,165,2213,1189,3237,677,2725,1701,3749,421,2469,1445,3493,933,2981,1957,4005,101,2149,1125,
3173,613,2661,1637,3685,357,2405,1381,3429,869,2917,1893,3941,229,2277,1253,3301,741,2789,1765,3813,485,2533,1509,3557,997,3045,2021,4069,21,2069,1045,3093,533,2581,1557,
3605,277,2325,1301,3349,789,2837,1813,3861,149,2197,1173,3221,661,2709,1685,3733,405,2453,1429,3477,917,2965,1941,3989,85,2133,1109,3157,597,2645,1621,3669,341,2389,1365,
3413,853,2901,1877,3925,213,2261,1237,3285,725,2773,1749,3797,469,2517,1493,3541,981,3029,2005,4053,53,2101,1077,3125,565,2613,1589,3637,309,2357,1333,3381,821,2869,1845,
3893,181,2229,1205,3253,693,2741,1717,3765,437,2485,1461,3509,949,2997,1973,4021,117,2165,1141,3189,629,2677,1653,3701,373,2421,1397,3445,885,2933,1909,3957,245,2293,1269,
3317,757,2805,1781,3829,501,2549,1525,3573,1013,3061,2037,4085,13,2061,1037,3085,525,2573,1549,3597,269,2317,1293,3341,781,2829,1805,3853,141,2189,1165,3213,653,2701,1677,
3725,397,2445,1421,3469,909,2957,1933,3981,77,2125,1101,3149,589,2637,1613,3661,333,2381,1357,3405,845,2893,1869,3917,205,2253,1229,3277,717,2765,1741,3789,461,2509,1485,
3533,973,3021,1997,4045,45,2093,1069,3117,557,2605,1581,3629,301,2349,1325,3373,813,2861,1837,3885,173,2221,1197,3245,685,2733,1709,3757,429,2477,1453,3501,941,2989,1965,
4013,109,2157,1133,3181,621,2669,1645,3693,365,2413,1389,3437,877,2925,1901,3949,237,2285,1261,3309,749,2797,1773,3821,493,2541,1517,3565,1005,3053,2029,4077,29,2077,1053,
3101,541,2589,1565,3613,285,2333,1309,3357,797,2845,1821,3869,157,2205,1181,3229,669,2717,1693,3741,413,2461,1437,3485,925,2973,1949,3997,93,2141,1117,3165,605,2653,1629,
3677,349,2397,1373,3421,861,2909,1885,3933,221,2269,1245,3293,733,2781,1757,3805,477,2525,1501,3549,989,3037,2013,4061,61,2109,1085,3133,573,2621,1597,3645,317,2365,1341,
3389,829,2877,1853,3901,189,2237,1213,3261,701,2749,1725,3773,445,2493,1469,3517,957,3005,1981,4029,125,2173,1149,3197,637,2685,1661,3709,381,2429,1405,3453,893,2941,1917,
3965,253,2301,1277,3325,765,2813,1789,3837,509,2557,1533,3581,1021,3069,2045,4093,3,2051,1027,3075,515,2563,1539,3587,259,2307,1283,3331,771,2819,1795,3843,131,2179,1155,
3203,643,2691,1667,3715,387,2435,1411,3459,899,2947,1923,3971,67,2115,1091,3139,579,2627,1603,3651,323,2371,1347,3395,835,2883,1859,3907,195,2243,1219,3267,707,2755,1731,
3779,451,2499,1475,3523,963,3011,1987,4035,35,2083,1059,3107,547,2595,1571,3619,291,2339,1315,3363,803,2851,1827,3875,163,2211,1187,3235,675,2723,1699,3747,419,2467,1443,
3491,931,2979,1955,4003,99,2147,1123,3171,611,2659,1635,3683,355,2403,1379,3427,867,2915,1891,3939,227,2275,1251,3299,739,2787,1763,3811,483,2531,1507,3555,995,3043,2019,
4067,19,2067,1043,3091,531,2579,1555,3603,275,2323,1299,3347,787,2835,1811,3859,147,2195,1171,3219,659,2707,1683,3731,403,2451,1427,3475,915,2963,1939,3987,83,2131,1107,
3155,595,2643,1619,3667,339,2387,1363,3411,851,2899,1875,3923,211,2259,1235,3283,723,2771,1747,3795,467,2515,1491,3539,979,3027,2003,4051,51,2099,1075,3123,563,2611,1587,
3635,307,2355,1331,3379,819,2867,1843,3891,179,2227,1203,3251,691,2739,1715,3763,435,2483,1459,3507,947,2995,1971,4019,115,2163,1139,3187,627,2675,1651,3699,371,2419,1395,
3443,883,2931,1907,3955,243,2291,1267,3315,755,2803,1779,3827,499,2547,1523,3571,1011,3059,2035,4083,11,2059,1035,3083,523,2571,1547,3595,267,2315,1291,3339,779,2827,1803,
3851,139,2187,1163,3211,651,2699,1675,3723,395,2443,1419,3467,907,2955,1931,3979,75,2123,1099,3147,587,2635,1611,3659,331,2379,1355,3403,843,2891,1867,3915,203,2251,1227,
3275,715,2763,1739,3787,459,2507,1483,3531,971,3019,1995,4043,43,2091,1067,3115,555,2603,1579,3627,299,2347,1323,3371,811,2859,1835,3883,171,2219,1195,3243,683,2731,1707,
3755,427,2475,1451,3499,939,2987,1963,4011,107,2155,1131,3179,619,2667,1643,3691,363,2411,1387,3435,875,2923,1899,3947,235,2283,1259,3307,747,2795,1771,3819,491,2539,1515,
3563,1003,3051,2027,4075,27,2075,1051,3099,539,2587,1563,3611,283,2331,1307,3355,795,2843,1819,3867,155,2203,1179,3227,667,2715,1691,3739,411,2459,1435,3483,923,2971,1947,
3995,91,2139,1115,3163,603,2651,1627,3675,347,2395,1371,3419,859,2907,1883,3931,219,2267,1243,3291,731,2779,1755,3803,475,2523,1499,3547,987,3035,2011,4059,59,2107,1083,
3131,571,2619,1595,3643,315,2363,1339,3387,827,2875,1851,3899,187,2235,1211,3259,699,2747,1723,3771,443,2491,1467,3515,955,3003,1979,4027,123,2171,1147,3195,635,2683,1659,
3707,379,2427,1403,3451,891,2939,1915,3963,251,2299,1275,3323,763,2811,1787,3835,507,2555,1531,3579,1019,3067,2043,4091,7,2055,1031,3079,519,2567,1543,3591,263,2311,1287,
3335,775,2823,1799,3847,135,2183,1159,3207,647,2695,1671,3719,391,2439,1415,3463,903,2951,1927,3975,71,2119,1095,3143,583,2631,1607,3655,327,2375,1351,3399,839,2887,1863,
3911,199,2247,1223,3271,711,2759,1735,3783,455,2503,1479,3527,967,3015,1991,4039,39,2087,1063,3111,551,2599,1575,3623,295,2343,1319,3367,807,2855,1831,3879,167,2215,1191,
3239,679,2727,1703,3751,423,2471,1447,3495,935,2983,1959,4007,103,2151,1127,3175,615,2663,1639,3687,359,2407,1383,3431,871,2919,1895,3943,231,2279,1255,3303,743,2791,1767,
3815,487,2535,1511,3559,999,3047,2023,4071,23,2071,1047,3095,535,2583,1559,3607,279,2327,1303,3351,791,2839,1815,3863,151,2199,1175,3223,663,2711,1687,3735,407,2455,1431,
3479,919,2967,1943,3991,87,2135,1111,3159,599,2647,1623,3671,343,2391,1367,3415,855,2903,1879,3927,215,2263,1239,3287,727,2775,1751,3799,471,2519,1495,3543,983,3031,2007,
4055,55,2103,1079,3127,567,2615,1591,3639,311,2359,1335,3383,823,2871,1847,3895,183,2231,1207,3255,695,2743,1719,3767,439,2487,1463,3511,951,2999,1975,4023,119,2167,1143,
3191,631,2679,1655,3703,375,2423,1399,3447,887,2935,1911,3959,247,2295,1271,3319,759,2807,1783,3831,503,2551,1527,3575,1015,3063,2039,4087,15,2063,1039,3087,527,2575,1551,
3599,271,2319,1295,3343,783,2831,1807,3855,143,2191,1167,3215,655,2703,1679,3727,399,2447,1423,3471,911,2959,1935,3983,79,2127,1103,3151,591,2639,1615,3663,335,2383,1359,
3407,847,2895,1871,3919,207,2255,1231,3279,719,2767,1743,3791,463,2511,1487,3535,975,3023,1999,4047,47,2095,1071,3119,559,2607,1583,3631,303,2351,1327,3375,815,2863,1839,
3887,175,2223,1199,3247,687,2735,1711,3759,431,2479,1455,3503,943,2991,1967,4015,111,2159,1135,3183,623,2671,1647,3695,367,2415,1391,3439,879,2927,1903,3951,239,2287,1263,
3311,751,2799,1775,3823,495,2543,1519,3567,1007,3055,2031,4079,31,2079,1055,3103,543,2591,1567,3615,287,2335,1311,3359,799,2847,1823,3871,159,2207,1183,3231,671,2719,1695,
3743,415,2463,1439,3487,927,2975,1951,3999,95,2143,1119,3167,607,2655,1631,3679,351,2399,1375,3423,863,2911,1887,3935,223,2271,1247,3295,735,2783,1759,3807,479,2527,1503,
3551,991,3039,2015,4063,63,2111,1087,3135,575,2623,1599,3647,319,2367,1343,3391,831,2879,1855,3903,191,2239,1215,3263,703,2751,1727,3775,447,2495,1471,3519,959,3007,1983,
4031,127,2175,1151,3199,639,2687,1663,3711,383,2431,1407,3455,895,2943,1919,3967,255,2303,1279,3327,767,2815,1791,3839,511,2559,1535,3583,1023,3071,2047,4095};
	for(int i = 0; i < 4096; i++)
	{
		if(i != exchange[i])
		{
			double temp = data[2*i];
			data[2*i] = data[2*exchange[i]];
			data[2*exchange[i]] = temp;
			temp = data[2*i+1];
			data[2*i+1] = data[2*exchange[i]+1];
			data[2*exchange[i]+1] = temp;
		}
	}

	nn = data.size();
	mmax = 2;	//Danielson-Lanczos FFT algoritm
	while(nn > mmax)
	{
		istep = mmax << 1;
		theta = isign*2.*M_PI/mmax;
		wtemp = sin(theta/2.);
		wpr = -2.*pow(wtemp,2);
		wpi = sin(theta);
		wr = 1.;
		wi = 0.;
		for(m = 1; m < mmax; m += 2)
		{
			for(i = m; i < nn; i += istep)
			{
				j = i + mmax;
				tempr = wr*data[j-1]-wi*data[j];
				tempi = wr*data[j]+wi*data[j-1];
				data[j-1] = data[i-1]-tempr;
				data[j] = data[i]-tempi;
				data[i-1] += tempr;
				data[i] += tempi;
			}
			wtemp = wr;
			wr = wtemp*wpr-wi*wpi+wr;
			wi = wi*wpr+wtemp*wpi+wi;
		}
		mmax = istep;
	}

	return(data);
}

double Track::Max()
{
	double Max = 0;

	for(double element: Data)
		if(Max < abs(element))
			Max = abs(element);

	return(Max);
}

double Track::Vec_Max(vector<double> data)
{
	double Max = 0;

	for(double element: data)
		if(Max < abs(element))
			Max = abs(element);

	return(Max);
}
