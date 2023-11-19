//This implementation of Hamming codes is a Hamming(511,502). 502 bits is enough for 62.75 bytes. I want to transmit 512 bits. So that's 7 bits not normally assigned. So I'm assigning parity bits to the remaining 7 bits for extra error detection.
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

uint512_t InsertZero(uint512_t number, int position)
{
	uint512_t mask = (uint512_t(1) << position) - 1;	//Create a mask to clear bits to the left of the position
	uint512_t leftBits = (number & (~mask)) << 1;		//Shift bits to the left by 1 position
	uint512_t rightBits = (number & mask);		//Preserve bits to the right of the position

	return(leftBits | rightBits);	//Combine both parts
}

int main()
{
	uint8_t Message[] = "Coding's canvas,errors do trace,Hamming restores,flaws erase" //This message is 6 bits short of what can be held in Hamming(511,502)+1
	uint512_t Encoding = 0;
	uint16_t Parity_Bits = 0;
	int i;

	for(i = 0; i < 62; i++)	//Load the message into Encoding. The for loop will exit with a byte of zeros on either end and 62 bytes of data in the middle.
	{
		Encoding += Message[i];
		Encoding <<= 8;
	}
	Encoding >>= 8;	//Now the encoding is 512 bit justified.
	Encoding = (Parity(Encoding, 0x0020a800021050001050100b004c3004008100090780000481010002000026322400204010a1040020640020c8020d000460000052400a09206184058084) << 6) |
	(Parity(Encoding, 0x82014200044126212028006000800400024044a41000150000008110800210001090022c0a0230050280840406001011000c32000810c060041c20580850) << 5) |
	(Parity(Encoding, 0x10000080102401120000ca8449008a015c2021408022609042000a090400000900600902814001400000210200058242080001dc85003410000009001003) << 4) |
	(Parity(Encoding, 0x000404662800008044042010100001400006081048000a2014a40404081101844206540144000208480842491010202c2202480020820184128040202400) << 3) |
	(Parity(Encoding, 0x69c00109000a880c09000500a0114028201082000001004a2852302010288840880080802014c09000131080206840008181040100090002010212000020) << 2) |
	Parity(Encoding, 0x041a1010c1800040828300000622009281081002205c8001000840c063c44000010900100008082295000810018000805010802200240000c80000824308) | (Encoding << 6);
	//Now the origal message is appended with 6 partity bits which are calculated off an random collection of bits with no overlap

	for(i = 0; i <= 8; i++)	//0 Bits inserted
		InsertZero(Encoding, 512-pow(2,i));

	//Parity bits to be inserted into the newly opened zero bit spaces
	Parity(Encoding, 0x155555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555);
	Parity(Encoding, 0x199999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999);
	Parity(Encoding, 0x078787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787);
	Parity(Encoding, 0x0000ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff);
	Parity(Encoding, 0x00007fff80007fff80007fff80007fff80007fff80007fff80007fff80007fff80007fff80007fff80007fff80007fff80007fff80007fff80007fff80007f);
	Parity(Encoding, 0x000000007fffffff800000007fffffff800000007fffffff800000007fffffff800000007fffffff800000007fffffff800000007fffffff800000007fffff);
	Parity(Encoding, 0x00000000000000007fffffffffffffff80000000000000007fffffffffffffff80000000000000007fffffffffffffff80000000000000007fffffffffffff);
	Parity(Encoding, 0x000000000000000000000000000000007fffffffffffffffffffffffffffffff800000000000000000000000000000007fffffffffffffffffffffffffffff);
	Parity(Encoding, 0x00000000000000000000000000000000000000000000000000000000000000007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff);
}





















