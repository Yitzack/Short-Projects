//This implementation of Hamming codes is a Hamming(511,502). 502 bits is enough for 62.75 bytes. I want to transmit 512 bits. So that's 7 bits not normally assigned when encoding an integer number of bytes. So I'm assigning parity bits to the remaining 7 bits for extra error detection. I expect the ability to correct 1 bit flip and identify between 3 and 9 bit flips.
#include<iostream>
#include<cstdint>
#include<boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost::multiprecision;

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
/*cout << hex << number << endl;
cout << mask << endl;
cout << leftBits << endl;
cout << rightBits << endl;
cout << (uint512_t(Bit) << position) << endl;
cout << (leftBits | rightBits | (uint512_t(Bit) << position)) << "\n" << endl;//*/
	return(leftBits | rightBits | (uint512_t(Bit) << position));	//Combine both parts with the bit inserted in between
}

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

int main()
{
	uint8_t Message[] = "Coding's canvas, errors do trace,Hamming restores,flaws erase"; //This message is 6 bits short of what can be held in Hamming(511,502)+1
	uint16_t Parity_Bits = 0;	//Parity bits called for by the Hamming(511,502) encoding
	uint512_t Encoding = 0;
	uint512_t BitMasks[] =
	{uint512_t("0x0020a800021050001050100b004c3004008100090780000481010002000026322400204010a1040020640020c8020d000460000052400a09206184058084"),
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
	int i;

	for(i = 0; i < 62; i++)	//Load the message into Encoding. The for loop will exit with a byte of zeros on either end and 62 bytes of data in the middle.
	{
		Encoding += Message[i];
		Encoding <<= 8;
	}
	Encoding >>= 8;	//Now the encoding is 512 bit justified.

	for(i = 0; i < 6; i++)	//Now the origal message is appended with 6 partity bits which are calculated off an random collection of bits with no overlap
		Encoding = (Encoding << 1) | Parity(Encoding, BitMasks[i]);

	for(i = 8; i >= 0; i--)	//0 Bits inserted
		Encoding = InsertBit(Encoding, 511-pow(2,i), false);

	for(i = 6; i < 15; i++)	//Extended Hamming Code parity bits inserted
		Encoding |= uint512_t(Parity(Encoding, BitMasks[i])) << 511-(1 << (i-6));
	Encoding |= uint512_t(Parity(Encoding, BitMasks[15])) << 511;

	Print(Encoding);	//62-bytes in Extended Hamming(511, 502) code plus 6 parity bits

	return(0);
}





















