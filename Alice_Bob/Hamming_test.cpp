//This implementation of Hamming codes is a Hamming(511,502). 502 bits is enough for 62.75 bytes. I want to transmit 512 bits. So that's 7 bits not normally assigned when encoding an integer number of bytes. So I'm assigning parity bits to the remaining 7 bits for extra error detection. I expect the ability to correct 1 bit flip and identify between 3 and 9 bit flips.
#include<iostream>
#include<cstdint>
#include<boost/multiprecision/cpp_int.hpp>
#include<random>
#include<cstring>
using namespace std;
using namespace boost::multiprecision;

const uint512_t BitMasks[] = {uint512_t("0x0020a800021050001050100b004c3004008100090780000481010002000026322400204010a1040020640020c8020d000460000052400a09206184058084"),
uint512_t("0x82014200044126212028006000800400024044a41000150000008110800210001090022c0a0230050280840406001011000c32000810c060041c20580850"),
uint512_t("0x10000080102401120000ca8449008a015c2021408022609042000a090400000900600902814001400000210200058242080001dc85003410000009001003"),
uint512_t("0x000404662800008044042010100001400006081048000a2014a40404081101844206540144000208480842491010202c2202480020820184128040202400"),
uint512_t("0x69c00109000a880c09000500a0114028201082000001004a2852302010288840880080802014c09000131080206840008181040100090002010212000020"),
uint512_t("0x041a1010c1800040828300000622009281081002205c8001000840c063c44000010900100008082295000810018000805010802200240000c80000824308"),
uint512_t("0x55555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555"),
uint512_t("0x33333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333"),
uint512_t("0x0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
uint512_t("0x00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff"),
uint512_t("0x0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff"),
uint512_t("0x00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff"),
uint512_t("0x0000000000000000ffffffffffffffff0000000000000000ffffffffffffffff0000000000000000ffffffffffffffff0000000000000000ffffffffffffffff"),
uint512_t("0x00000000000000000000000000000000ffffffffffffffffffffffffffffffff00000000000000000000000000000000ffffffffffffffffffffffffffffffff"),
uint512_t("0x0000000000000000000000000000000000000000000000000000000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"),
uint512_t("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")};

void Print(const uint512_t value)
{
	constexpr int bytesInLine = 16;
	constexpr int bytesInGroup = 4;

	uint512_t mask = 1;

	for(int i = 512; i > 0; --i)
	{
		cout << ((value >> (i-1)) & mask);// << " ";

		if((512-i) % 128 == 127)
			cout << endl;
		else if((512-i) % 32 == 31)
			cout << "|";
		else if((512-i) % 8 == 7)
			cout << " ";//*/
	}
	//cout << endl;
}

bool Parity(uint512_t num, uint512_t mask)
{
	uint512_t maskedBits = num & mask;	//Extract only the specified bits

	bool parity = false;
	while (maskedBits)	//Loop only executes as many times as there are set bits
	{
		parity = !parity;	//Flip parity every time there's a set bit
		maskedBits = maskedBits & (maskedBits - 1);	//Clear the least significant set bit. For example 0b01010000 & 0b01001111 = 0b01000000
	}

	return(parity);
}

uint512_t InsertBit(uint512_t number, int position, bool Bit)
{
	uint512_t mask = (uint512_t(1) << position) - 1;	//Create a mask to clear bits to the left of the position
	uint512_t leftBits = (number & (~mask)) << 1;		//Shift bits to the left by 1 position
	uint512_t rightBits = (number & mask);		//Preserve bits to the right of the position

	return(leftBits | rightBits | (uint512_t(Bit) << position));	//Combine both parts with the bit inserted in between
}

uint512_t RemoveBit(uint512_t number, int position)
{
	uint512_t mask = (uint512_t(1) << position) - 1;	//Create a mask to clear bits to the left of the position
	uint512_t leftBits = (number & (~mask)) >> 1;		//Shift bits to the left by 1 position
	uint512_t rightBits = (number & mask);		//Preserve bits to the right of the position

	return(leftBits | rightBits);	//Combine both parts
}

uint512_t Encode(uint8_t Message[])
{
	uint512_t Encoding = 0;
	int i;

	for(i = 0; i < 62; i++)	//Load the message into Encoding. The for loop will exit with a byte of zeros on either end and 62 bytes of data in the middle.
	{
		Encoding += Message[i];
		Encoding <<= 8;
	}
	Encoding >>= 8;	//Now the encoding is 512 bit justified.

	for(i = 0; i < 6; i++)	//Now the origal message is appended with 6 partity bits which are calculated off an random collection of bits with no overlap
		Encoding = (Encoding << 1) | Parity(Encoding, BitMasks[i] << i);

	for(i = 8; i >= 0; i--)	//0 Bits inserted
		Encoding = InsertBit(Encoding, 511-pow(2,i), false);

	for(i = 6; i < 15; i++)	//Extended Hamming Code parity bits inserted
		Encoding |= uint512_t(Parity(Encoding, BitMasks[i])) << 511-(1 << (i-6));
	Encoding |= uint512_t(Parity(Encoding, BitMasks[15])) << 511;

	return(Encoding);
}

uint512_t FlipBit(uint512_t num, int Position)
{
	// Flipping the bit
	uint512_t mask = (uint512_t(1) << Position);

	return(num ^ mask);
}

bool Decode(uint512_t Encoding, uint8_t Message[])
{
	int i;
	uint16_t Parity_bit_error = 0;	//Location of flipped bit
	bool Total_Parity;			//Total parity

	for(i = 14; i >= 6; i--)
	{
		Parity_bit_error += Parity(Encoding, BitMasks[i]);
		Parity_bit_error <<= 1;
	}
	Parity_bit_error >>= 1;
	Total_Parity = Parity(Encoding, BitMasks[15]);

	if(Total_Parity)	//If odd Parity, there is a chance for correction, so make it
		Encoding = FlipBit(Encoding, 511-Parity_bit_error);

	Encoding = RemoveBit(Encoding, 512);	//Remove the normal extended Hamming(511,502) parity bits
	for(i = 0; i < 9; i++)
		Encoding = RemoveBit(Encoding, 512-(1<<i));

	uint8_t Extra_Parity = uint8_t(Encoding & 0x3F);	//Pull the extra parity bits and hopefully restore the original encoding
	Encoding >>= 6;

	uint8_t Extra_Parity_Check = 0;	//Calculate the current extra parity bits
	for(i = 0; i < 6; i++)
		Extra_Parity_Check += (Parity(Encoding, BitMasks[i]) << 5-i);

	for(i = 61; i >= 0; i--)	//Load the Encoding into recovered
	{
		Message[i] = uint8_t(Encoding & 0xFF);
		Encoding >>= 8;
	}

	return(!(Extra_Parity^Extra_Parity_Check));
}

int main()
{
	uint8_t Message[] = "Coding's canvas, errors do trace,Hamming restores,flaws erase"; //This message is 6 bits short of what can be held in Hamming(511,502)+1
	uint8_t Recovered[62];
	bool Success[2];
	uint512_t Encoding = Encode(Message);
	int i, j = 0;

	random_device rd;	//RNG for random bit flipping
	mt19937 gen(rd());
	uniform_int_distribution<int> distrib(0, 511);

	for(i = 0; i <= 20; i++)
	{
		Success[0] = Decode(Encoding, Recovered);	//Attempt to recover from the randomly flipped bit
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

		Encoding = FlipBit(Encoding, distrib(gen));
	}

	for(; i <= 100000; i+=2)
	{
		Success[0] = Decode(Encoding, Recovered);	//Attempt to recover from the randomly flipped bit
		Success[1] = !memcmp(Message, Recovered, 62);

		if(Success[0] ^ Success[1])
			j++;

		Encoding = FlipBit(Encoding, distrib(gen));
		Encoding = FlipBit(Encoding, distrib(gen));
	}

	cout << "In " << i-1 << " bit flips, " << j << " valid Hamming codes were found errounously, or about " << float(j)/float(i-1)*100 << "% failure rate." << endl;

	return(0);
}
