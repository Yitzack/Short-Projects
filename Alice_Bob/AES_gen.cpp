#include<iostream>
#include<random>
#include<cstdint>	//defines data type uint8_t for 1-byte (8-bit) integers
#include<boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost::multiprecision;

void Construct_SBox();
void Construct_InvSBox();
uint8_t circularLeftShift(uint8_t, int);
void Rotate_Word(uint8_t *, int);
void Sub_Word(uint8_t *);
void Inv_Rotate_Word(uint8_t *, int);
void Inv_Sub_Word(uint8_t *);

class GF256
{
	public:
		GF256();
		GF256(uint8_t);
		GF256 operator=(GF256);
		GF256 operator+(GF256);
		GF256 operator-(GF256);
		GF256 operator*(GF256);
		GF256 operator/(GF256);
		friend ostream& operator<<(ostream&, const GF256&);
	private:
		uint8_t Byte;
};

GF256::GF256()
{
	Byte = 0;
	return;
}

GF256::GF256(uint8_t x)
{
	Byte = x;
	return;
}

GF256 GF256::operator=(GF256 x)
{
	Byte = x.Byte;
	return(Byte);
}

GF256 GF256::operator+(GF256 x)
{
	Byte ^= x.Byte;
	return(Byte);
}

GF256 GF256::operator*(GF256 x)
{
	uint8_t temp = Byte;
	uint8_t answer = 0;

	for(int i = 0; i < 8; i++)
	{
		if((x.Byte&1))
		{
			answer ^= temp;
		}
		x.Byte >>= 1;

		if(temp&0x80)
			temp = (temp << 1) ^ 0x1B;
		else
			temp <<= 1;
	}

	return(GF256(answer));
}

ostream& operator<<(ostream& os, const GF256& A)
{
	os << int(A.Byte);
	return(os);
}


uint8_t SBox[256];
uint8_t InvSBox[256];

int main()
{
	uint8_t Key8[] = {0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81, 0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4};	//32-block key of 8-bit blocks
	uint32_t Key32[] = {0x603DEB10, 0x15CA71BE, 0x2B73AEF0, 0x857D7781, 0x1F352C07, 0x3B6108D7, 0x2D9810A3, 0x0914DFF4};	//8-block key of 32-bit blocks
	uint32_t PlainText[] = {0x6BC1BEE2, 0x2E409F96, 0xE93D7E11, 0x7393172A, 0xAE2D8A57, 0x1E03AC9C, 0x9EB76FAC, 0x45AF8E51, 0x30C81C46, 0xA35CE411, 0xE5FBC119, 0x1A0A52EF, 0xF69F2445, 0xDF4F9B17, 0xAD2B417B, 0xE66C3710};	//Plain text to test with
	uint32_t CipherTextCorrect[] = {0xF3EED1BD, 0xB5D2A03C, 0x064B5A7E, 0x3DB181F8, 0x591CCB10, 0xD410ED26, 0xDC5BA74A, 0x31362870, 0xB6ED21B9, 0x9CA6F4F9, 0xF153E7B1, 0xBEAFED1D, 0x23304B7A, 0x39F9F3FF, 0x067D8D8F, 0x9E24ECC7};	//Correct cipher text
	uint32_t CipherTextTest[16];	//Place to store result of encyrption
	uint32_t PlainTextTest[16];	//Place to store result of decryption

	Construct_SBox();	//Construct the SBox. SBox is global and so this function has a side effect.. constructing the SBox
	Construct_InvSBox();	//Construct the InvSBox. InvSBox is global and so this function has a side effect.. constructing the InvSBox

	return(0);
}

void Rotate_Word(uint8_t * Word, int Shift)	//Useful for ShiftRows() and RotWord() in specification. Shift to the left Shift elements. Word had better be 4 bytes or there will be problems.
{
	uint8_t temp[2];

	switch(Shift%4)
	{
	case 3:
		temp[0] = Word[3];
		Word[3] = Word[2];
		Word[2] = Word[1];
		Word[1] = Word[0];
		Word[0] = temp[0];
		break;
	case 2:
		temp[0] = Word[2];
		temp[1] = Word[3];
		Word[3] = Word[1];
		Word[2] = Word[0];
		Word[1] = temp[1];
		Word[0] = temp[0];
		break;
	case 1:
		temp[0] = Word[0];
		Word[0] = Word[1];
		Word[1] = Word[2];
		Word[2] = Word[3];
		Word[3] = temp[0];
		break;
	}

	/*Shift = Shift % 4;	//ChatGPT suggested this code for the code above. The looping is probably slower than the switch/case above. It could take a while to figure that out.
	uint8_t temp;

	for(int i = 0; i < Shift; i++)	//Shift the specified number of times
	{
		temp = Word[0];
		for (int j = 0; j < 3; j++)	//Shift left
		{
			Word[j] = Word[j+1];
		}
		Word[3] = temp;
	}*/
}

void Sub_Word(uint8_t * Word)	//Substitute from the SBox for 4 bytes
{
	for(int i = 0; i < 4; i++)
		Word[i] = SBox[Word[i]];
}

void Inv_Rotate_Word(uint8_t * Word, int Shift)	//Useful for ShiftRows() and RotWord() in specification. Shift to the left Shift elements. Word had better be 4 bytes or there will be problems.
{
	uint8_t temp[2];

	switch(Shift%4)
	{
	case 1:
		temp[0] = Word[3];
		Word[3] = Word[2];
		Word[2] = Word[1];
		Word[1] = Word[0];
		Word[0] = temp[0];
		break;
	case 2:
		temp[0] = Word[2];
		temp[1] = Word[3];
		Word[3] = Word[1];
		Word[2] = Word[0];
		Word[1] = temp[1];
		Word[0] = temp[0];
		break;
	case 3:
		temp[0] = Word[0];
		Word[0] = Word[1];
		Word[1] = Word[2];
		Word[2] = Word[3];
		Word[3] = temp[0];
		break;
	}

	/*Shift = Shift % 4;	//ChatGPT suggested this code for the code above. The looping is probably slower than the switch/case above. It could take a while to figure that out.
	uint8_t temp;

	for(int i = 0; i < Shift; i++)	//Shift the specified number of times
	{
		temp = Word[0];
		for (int j = 0; j < 3; j++)	//Shift left
		{
			Word[j+1] = Word[j];
		}
		Word[3] = temp;
	}*/
}

void Inv_Sub_Word(uint8_t * Word)	//Substitute from the SBox for 4 bytes
{
	for(int i = 0; i < 4; i++)
		Word[i] = InvSBox[Word[i]];
}

uint8_t circularLeftShift(uint8_t num, int shift)
{
	return((num << shift) | (num >> (8 - shift)));
}

void Construct_SBox()
{
	GF256 p(1), q(1);
	
	do
	{
		//Multiply by x+1%(x^8+x^4+x^3+x+1) in GF(256), x+1 is 3 and coprime with 256. (p*=GF256(0x03))
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		//Divide by x+1%(x^8+x^4+x^3+x+1), which is equivalent to multiplying by x^7+x^6+x^5+x^4+x^2+x%(x^8+x^4+x^3+x+1) in GF(256),  (q*=GF256(0xf6))
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		//Compute the affine transformation
		SBox[p] = q ^ circularLeftShift(q, 1) ^ circularLeftShift(q, 2) ^ circularLeftShift(q, 3) ^ circularLeftShift(q, 4) ^ 0x63;
	}while(p != 1);

	//0 is a special case since it has no inverse
	SBox[0] = 0x63;
}

void Construct_InvSBox()
{
	for(int i = 0; i < 256; i++)
		InvSBox[SBox[i]] = i;
}
