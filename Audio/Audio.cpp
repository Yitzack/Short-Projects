#include<fstream>
#include<climits>
#include"Track.h"
using namespace std;

void Write_Num(ostream &, uint16_t);	//Writes data out littlendian instead of the default bigendian
void Write_Num(ostream &, double);
void Write_Num(ostream &, uint32_t);
void Write_WAV(Track, Track);

int main()
{
	Track Left;
	Track Right;
	double Max;

	Left.Sine(1.,440.,1,0);
	Track fft = Left.FFT();
	Right = fft.IFFT();

	/*Max = Left.Max();
	Left.Fader(-20.*log(Max)/log(10.));
	Max = Right.Max();
	Right.Fader(-20.*log(Max)/log(10.));*/

	Write_WAV(Left, Right);

	return(0);
}

void Write_WAV(Track Left, Track Right)
{
	//Constants for writting a Wave file
	const char Chunk_descriptor_ID[5] = "RIFF";
	const char Chunk_descriptor_Fmt[5] = "WAVE";
	const char SubChunk1ID[5] = "fmt ";
	const uint8_t FormatSubChunkData[] = {16,0,0,0,1,0};	//Subchunk is 16 bytes, Audio format is LPCM
	const uint16_t NumChannels = 2;
	const uint16_t BitsPerSample = 16;	//Bitdepth per sample (int16_t)
	const uint32_t ByteRate = SAMPLE_RATE*NumChannels*BitsPerSample/8;
	const uint16_t BlockAlign = NumChannels*BitsPerSample/8;
	const char SubChunk2ID[5] = "data";

	//Size for part of the Wave file format
	uint32_t SubChunk2Size = 2*Left.Length()*BitsPerSample/8;
	uint32_t Chunk_Size = 32+SubChunk2Size;

	//Write the header to file
	ofstream File("Audio.wav");
	File << Chunk_descriptor_ID;
	Write_Num(File, Chunk_Size);
	File << Chunk_descriptor_Fmt;
	File << SubChunk1ID;
	for(int i = 0; i < 6; i++)
		File << FormatSubChunkData[i];
	Write_Num(File, NumChannels);
	Write_Num(File, uint32_t(SAMPLE_RATE));
	Write_Num(File, ByteRate);
	Write_Num(File, BlockAlign);
	Write_Num(File, BitsPerSample);
	File << SubChunk2ID;
	Write_Num(File, SubChunk2Size);

	//Write the data to file
	for(int i = 0; i < min(Left.Length(),Right.Length()); i++)
	{
		Write_Num(File, Left[i]);
		Write_Num(File, Right[i]);
	}

	File << flush;
	File.close();
}

void Write_Num(ostream & out, uint16_t data)
{
	out << uint8_t(data & 0x00FF) << uint8_t((data & 0xFF00) >> 8);
}

void Write_Num(ostream & out, double in_data)	//track data is stored as double. It needs to discritized into int_16 form for writting to disk
{
	int16_t data;

	if(in_data > 1)
		data = SHRT_MAX;
	else if(in_data < -1)
		data = SHRT_MIN;
	else
		data = in_data*SHRT_MAX;

	out << uint8_t(data & 0x00FF) << uint8_t((data & 0xFF00) >> 8);
}

void Write_Num(ostream & out, uint32_t data)
{
	out << uint8_t(data & 0x000000FF) << uint8_t((data & 0x0000FF00) >> 8) << uint8_t((data & 0x00FF0000) >> 16) << uint8_t((data & 0xFF000000) >> 24);
}
