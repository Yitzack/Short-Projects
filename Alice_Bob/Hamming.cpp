#include<boost/multiprecision/cpp_int.hpp>
#include"Hamming.h"
using namespace boost::multiprecision;

Hamming::Hamming(uint8_t Message[])
{
	Encode(Message);
}

void Hamming::Encode(uint8_t Message[])
{
	Encoding = 0;
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
}

bool Hamming::Decode(uint8_t Message[]) const
{
	return(Decode(Encoding,Message));
}

bool Hamming::Decode(uint512_t Coding, uint8_t Message[]) const
{
	int i;
	uint16_t Parity_bit_error = 0;	//Location of flipped bit
	bool Total_Parity;			//Total parity

	for(i = 14; i >= 6; i--)
	{
		Parity_bit_error += Parity(Coding, BitMasks[i]);
		Parity_bit_error <<= 1;
	}
	Parity_bit_error >>= 1;
	Total_Parity = Parity(Coding, BitMasks[15]);

	if(Total_Parity)	//If odd Parity, there is a chance for correction, so make it
		Coding = FlipBit(Coding, 511-Parity_bit_error);

	Coding = RemoveBit(Coding, 512);	//Remove the normal extended Hamming(511,502) parity bits
	for(i = 0; i < 9; i++)
		Coding = RemoveBit(Coding, 512-(1<<i));

	uint8_t Extra_Parity = uint8_t(Coding & 0x3F);	//Pull the extra parity bits and hopefully restore the original encoding
	Coding >>= 6;

	uint8_t Extra_Parity_Check = 0;	//Calculate the current extra parity bits
	for(i = 0; i < 6; i++)
		Extra_Parity_Check += (Parity(Coding, BitMasks[i]) << 5-i);

	for(i = 61; i >= 0; i--)	//Load the Encoding into recovered
	{
		Message[i] = uint8_t(Coding & 0xFF);
		Coding >>= 8;
	}

	return(!(Extra_Parity^Extra_Parity_Check));
}

void Hamming::Set_Encoding(uint512_t Code)
{
	Encoding = Code;
	return;
}

void Hamming::Set_Encoding(Hamming Code)
{
	Encoding = Code.Encoding;
	return;
}

uint512_t Hamming::Export_Encoding() const
{
	return(Encoding);
}

bool Hamming::Parity(uint512_t num, uint512_t mask) const
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

uint512_t Hamming::InsertBit(uint512_t number, int position, bool Bit) const
{
	uint512_t mask = (uint512_t(1) << position) - 1;	//Create a mask to clear bits to the left of the position
	uint512_t leftBits = (number & (~mask)) << 1;		//Shift bits to the left by 1 position
	uint512_t rightBits = (number & mask);		//Preserve bits to the right of the position

	return(leftBits | rightBits | (uint512_t(Bit) << position));	//Combine both parts with the bit inserted in between
}

uint512_t Hamming::RemoveBit(uint512_t number, int position) const
{
	uint512_t mask = (uint512_t(1) << position) - 1;	//Create a mask to clear bits to the left of the position
	uint512_t leftBits = (number & (~mask)) >> 1;		//Shift bits to the left by 1 position
	uint512_t rightBits = (number & mask);		//Preserve bits to the right of the position

	return(leftBits | rightBits);	//Combine both parts
}

uint512_t Hamming::FlipBit(uint512_t num, int Position) const
{
	// Flipping the bit
	uint512_t mask = (uint512_t(1) << Position);

	return(num ^ mask);
}

void Hamming::FlipBit(int Position)
{
	// Flipping the bit
	uint512_t mask = (uint512_t(1) << Position);

	Encoding ^= mask;
}
