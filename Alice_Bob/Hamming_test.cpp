#include<iostream>
#include<cstdint>
#include<boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost::multiprecision;

bool Parity(cpp_int num, cpp_int mask)
{
	cpp_int maskedBits = num & mask;	//Extract only the specified bits

	bool parity = false;
	while (maskedBits)	//Loop only executes as many times as there are set bits
	{
		parity = !parity;	//Flip parity every time there's a set bit
		maskedBits = maskedBits & (maskedBits - 1);	//Clear the least significant set bit. For example 0b01010000 & 0b01001111 = 0b01000000
	}

	return(parity);
}

int main()
{
	uint8_t Message[] = "Coding's canvas,errors do trace,Hamming restores,flaws erase" //This message is 6 bits short of what can be held in Hamming(511,502)+1
}
