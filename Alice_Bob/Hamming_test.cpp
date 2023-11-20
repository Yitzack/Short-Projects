//This implementation of Hamming codes is a Hamming(511,502). 502 bits is enough for 62.75 bytes. I want to transmit 512 bits. So that's 7 bits not normally assigned when encoding an integer number of bytes. So I'm assigning parity bits to the remaining 7 bits for extra error detection. I expect the ability to correct 1 bit flip and identify between 3 and 9 bit flips.
#include<iostream>
#include<cstdint>
#include<random>
#include<cstring>
#include"Hamming.h"
using namespace std;

int main()
{
	uint8_t Message[] = "Coding's canvas, errors do trace,Hamming restores,flaws erase"; //This message is 6 bits short of what can be held in Hamming(511,502)+1
	uint8_t Recovered[62];
	bool Success[2];
	Hamming Encoding(Message);
	int i, j = 0;

	random_device rd;	//RNG for random bit flipping
	mt19937 gen(rd());
	uniform_int_distribution<int> distrib(0, 511);

	for(i = 0; i <= 20; i++)
	{
		Success[0] = Encoding.Decode(Recovered);	//Attempt to recover from the randomly flipped bit
		Success[1] = !memcmp(Message, Recovered, 62);

		cout << "Detecting or recovering from " << i << " flipped bits.\n";
		if(Success[0])
			cout << "A valid Hamming code was recovered.\n";
		else
			cout << "An invalid Hamming code was recovered.\n";
		if(Success[1])
			cout << "The correct string was recovered.\n";
		else
			cout << "The correct string wasn't recovered.\n";
		if(!(Success[0] ^ Success[1]))
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

		if(Success[0] ^ Success[1])
			j++;

		Encoding.FlipBit(distrib(gen));
	}

	cout << "In " << i-1 << " bit flips, " << j << " valid Hamming codes were found errounously, or about " << float(j)/float(i-1)*100 << "% failure rate." << endl;

	return(0);
}
