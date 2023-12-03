//This implementation of Hamming codes is a Hamming(511,502). 502 bits is enough for 62.75 bytes. I want to transmit 512 bits. So that's 7 bits not normally assigned when encoding an integer number of bytes. So I'm assigning parity bits to the remaining 7 bits for extra error detection. I expect the ability to correct 1 bit flip and identify between 3 and 9 bit flips.
#include<iostream>
#include<cstdint>
#include<random>
#include<cstring>
#include"Hamming.h"
using namespace std;

int main()
{
	uint8_t Randomish[2][62] = {{0x43, 0x6f, 0x64, 0x69, 0x6e, 0x67, 0x27, 0x73, 0x20, 0x63, 0x61, 0x6e, 0x76, 0x61, 0x73, 0x2c, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x73, 0x20, 0x64, 0x6f, 0x20, 0x74, 0x72, 0x61, 0x63, 0x65, 0x2c, 0x48, 0x61, 0x6d, 0x6d, 0x69, 0x6e, 0x67, 0x20, 0x72, 0x65, 0x73, 0x74, 0x6f, 0x72, 0x65, 0x73, 0x2c, 0x66, 0x6c, 0x61, 0x77, 0x73, 0x20, 0x65, 0x72, 0x61, 0x73, 0x65, 0x00},
	{0x65, 0xb7, 0xe2, 0xca, 0xc7, 0x27, 0x66, 0xfd, 0xf8, 0xe2, 0x13, 0xf5, 0xe4, 0x13, 0x6f, 0xc0, 0xdf, 0x26, 0x2c, 0x98, 0x20, 0x92, 0x4a, 0xd0, 0xbd, 0xf4, 0x42, 0x6e, 0x53, 0xfb, 0xce, 0x92, 0xff, 0x89, 0x20, 0xc6, 0x1b, 0x92, 0xa1, 0x69, 0x8a, 0xbc, 0x8c, 0x47, 0xda, 0x36, 0xf1, 0xe2, 0x24, 0x2e, 0x0c, 0xea, 0x9b, 0x4c, 0x00, 0x2a, 0x91, 0x16, 0x85, 0x3e, 0xbb, 0x5b}};//"Coding's canvas, errors do trace,Hamming restores,flaws erase";//This message is 6 bits short of what can be held in Hamming(511,502)+1
	uint8_t Message[18][62];
	uint8_t Recovered[62];
	int Success[2];
	int i, j = 0;

	for(i = 0; i < 16; i++)
		for(j = 0; j < 62; j++)
			Message[i][j] = i;
	for(i; i < 18; i++)
		for(j = 0; j < 62; j++)
			Message[i][j] = Randomish[i-16][j];

	Hamming Encoding;

	random_device rd;	//RNG for random bit flipping
	mt19937 gen(rd());
	uniform_int_distribution<int> distrib(0, 511);

	for(i = 0; i < 18; i++)
	{
		Encoding.Encode(Message[i]);
		Success[0] = Encoding.Decode(Recovered);	//Attempt to recover from the randomly flipped bit
		Success[1] = memcmp(Message[i], Recovered, 62);

		cout << "Detecting or recovering from message " << i << ".\n";
		if(Success[0])
			cout << "A valid Hamming code was recovered.\n";
		else
			cout << "An invalid Hamming code was recovered.\n";

		if(Success[1] == 0)
			cout << "The correct string was recovered.\n";
		else
			cout << "The correct string wasn't recovered.\n";
		if(!(Success[0] ^ (Success[1]==0)))
			cout << "The Hamming code error detection or recovery was successful.\n" << endl;
		else
		{
			cout << "The Hamming code error detection or recovery was unsuccessful.\n" << endl;
			j++;
		}
	}

	for(i = 0; i < 20; i++)
	{
		Success[0] = Encoding.Decode(Recovered);	//Attempt to recover from the randomly flipped bit
		Success[1] = memcmp(Message[17], Recovered, 62);

		cout << "Detecting or recovering from " << i << " flipped bits.\n";
		if(Success[0])
			cout << "A valid Hamming code was recovered.\n";
		else
			cout << "An invalid Hamming code was recovered.\n";
		if(Success[1] == 0)
			cout << "The correct string was recovered.\n";
		else
			cout << "The correct string wasn't recovered.\n";
		if(!(Success[0] ^ (Success[1]==0)))
			cout << "The Hamming code error detection or recovery was successful.\n" << endl;
		else
		{
			cout << "The Hamming code error detection or recovery was unsuccessful.\n" << endl;
			j++;
		}

		Encoding.FlipBit(distrib(gen));
	}

	for(; i <= 100000; i++)
	{
		Success[0] = Encoding.Decode(Recovered);	//Attempt to recover from the randomly flipped bit
		Success[1] = !memcmp(Message, Recovered, 62);

		if(!(Success[0] ^ (Success[1]==0)))
			j++;

		Encoding.FlipBit(distrib(gen));
	}

	cout << "In " << i-1 << " bit flips, " << j << " valid Hamming codes were found errounously, or about " << float(j)/float(i-1)*100 << "% failure rate." << endl;

	return(0);
}
