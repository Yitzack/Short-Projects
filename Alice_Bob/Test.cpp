#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
using namespace boost::multiprecision;
using namespace std;

void Hexer(char* Hex_Code, cpp_int Number)
{
	const char hexDigits[] = "0123456789abcdef";
	int digit;
	int Length;
	int i = 0;

	while((Number >> i) > cpp_int(0))
	{
		i+=4;
	}
	Length = i/4-1;
	i /= 4;

	while(i != 0)
	{
		digit = int((Number >> 4*i) & 0xF); // Extract 4 bits at a time
		Hex_Code[Length-i] = hexDigits[digit];
		i--;
	}
	Hex_Code[Length+1] = char(0);
}

int main()
{
	// Repeat at arbitrary precision:
	cpp_int u = 1;
	for(unsigned i = 1; i <= 600; ++i)
		u *= i;

	char String[1200];
	for(int i = 0; i < 1200; i++)
		String[i] = 0;
	Hexer(String, u);

	// prints 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000 (i.e. 100!)
	cout << u << endl;
	cout << hex << u << endl;
	cout << String << endl;

	return 0;
}

