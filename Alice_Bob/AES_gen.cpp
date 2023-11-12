#include<iostream>
#include<iomanip>
#include<random>
#include<cstdint>	//defines data type uint8_t for 1-byte (8-bit) integers
#include<boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost::multiprecision;

class GF256	// Galois Field(256) implementation
{
	public:
		GF256();
		GF256(uint8_t);
		uint8_t to_int();
		GF256 operator=(GF256);
		GF256 operator=(uint8_t);
		bool operator==(GF256);
		bool operator==(uint8_t);
		bool operator!=(GF256);
		bool operator!=(uint8_t);
		GF256 operator+(GF256);
		GF256 operator+(uint8_t);
		GF256 operator-(GF256);
		GF256 operator-(uint8_t);
		GF256 operator*(GF256);
		GF256 operator*(uint8_t);
		GF256 operator^(GF256);
		GF256 operator^(uint8_t);
		GF256 operator|(GF256);
		GF256 operator|(uint8_t);
		GF256 operator&(GF256);
		GF256 operator&(uint8_t);
		//GF256 operator/(GF256); I don't think I need this one.
		friend ostream& operator<<(ostream&, const GF256&);
		GF256 operator<<(uint8_t);
		GF256 operator>>(uint8_t);
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

uint8_t GF256::to_int()
{
	return(Byte);
}

GF256 GF256::operator=(GF256 x)
{
	Byte = x.Byte;
	return(Byte);
}

GF256 GF256::operator=(uint8_t x)
{
	Byte = x;
	return(GF256(x));
}

bool GF256::operator==(GF256 x)
{
	return(Byte == x.Byte);
}

bool GF256::operator==(uint8_t x)
{
	return(Byte == x);
}

bool GF256::operator!=(GF256 x)
{
	return(Byte != x.Byte);
}

bool GF256::operator!=(uint8_t x)
{
	return(Byte != x);
}

GF256 GF256::operator+(GF256 x)
{
	uint8_t answer = Byte ^ x.Byte;
	return(GF256(answer));
}

GF256 GF256::operator+(uint8_t x)
{
	uint8_t answer = Byte ^ x;
	return(GF256(answer));
}

GF256 GF256::operator-(GF256 x)
{
	uint8_t answer = Byte ^ ~x.Byte;
	return(GF256(answer));
}

GF256 GF256::operator-(uint8_t x)
{
	uint8_t answer = Byte ^ ~x;
	return(GF256(answer));
}

GF256 GF256::operator^(GF256 x)
{
	uint8_t answer = Byte ^ x.Byte;
	return(GF256(answer));
}

GF256 GF256::operator^(uint8_t x)
{
	uint8_t answer = Byte ^ x;
	return(GF256(answer));
}

GF256 GF256::operator|(GF256 x)
{
	uint8_t answer = Byte | x.Byte;
	return(GF256(answer));
}

GF256 GF256::operator|(uint8_t x)
{
	uint8_t answer = Byte | x;
	return(GF256(answer));
}

GF256 GF256::operator&(GF256 x)
{
	uint8_t answer = Byte & x.Byte;
	return(GF256(answer));
}

GF256 GF256::operator&(uint8_t x)
{
	uint8_t answer = Byte & x;
	return(GF256(answer));
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

GF256 GF256::operator*(uint8_t x)
{
	uint8_t temp = Byte;
	uint8_t answer = 0;

	for(int i = 0; i < 8; i++)
	{
		if((x&1))
		{
			answer ^= temp;
		}
		x >>= 1;

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

GF256 GF256::operator<<(uint8_t shift)
{
	return(GF256(Byte<<shift));
}

GF256 GF256::operator>>(uint8_t shift)
{
	return(GF256(Byte>>shift));
}

void Construct_SBox();
void Construct_InvSBox();
GF256 circularLeftShift(GF256, int);
void Rotate_Word(GF256 *, int);
void Sub_Word(GF256 *);
void Inv_Rotate_Word(GF256 *, int);
void Inv_Sub_Word(GF256 *);
GF256* Key_Expansion(uint8_t[]);
void KeyAddition(uint32_t[], GF256*, int, int);

GF256 SBox[256];
GF256 InvSBox[256];
const int Nk = 8;	//Number of 4-byte words that make up the AES-256 key.
const int Nr = 14;	//Number of rounds taken in AES-256

int main()
{
	uint8_t Key[] = {0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81, 0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4};	//32-block key of 8-bit blocks
	uint32_t PlainText[] = {0x6BC1BEE2, 0x2E409F96, 0xE93D7E11, 0x7393172A, 0xAE2D8A57, 0x1E03AC9C, 0x9EB76FAC, 0x45AF8E51, 0x30C81C46, 0xA35CE411, 0xE5FBC119, 0x1A0A52EF, 0xF69F2445, 0xDF4F9B17, 0xAD2B417B, 0xE66C3710};	//Plain text to test with
	uint32_t CipherTextCorrect[] = {0xF3EED1BD, 0xB5D2A03C, 0x064B5A7E, 0x3DB181F8, 0x591CCB10, 0xD410ED26, 0xDC5BA74A, 0x31362870, 0xB6ED21B9, 0x9CA6F4F9, 0xF153E7B1, 0xBEAFED1D, 0x23304B7A, 0x39F9F3FF, 0x067D8D8F, 0x9E24ECC7};	//Correct cipher text
	uint32_t* CipherTextTest;	//Place to store result of encyrption
	uint32_t* PlainTextTest;	//Place to store result of decryption
	memcpy(CipherTextTest, PlainText, 16*sizeof(uint32_t));

	cout << hex;

	Construct_SBox();	//Construct the SBox. SBox is global and so this function has a side effect.. constructing the SBox
	Construct_InvSBox();	//Construct the InvSBox. InvSBox is global and so this function has a side effect.. constructing the InvSBox
	GF256* Expanded_Key = Key_Expansion(Key);	//Expand out the key

	KeyAddition(CipherTextTest, Expanded_Key, 0, 0);
	KeyAddition(CipherTextTest, Expanded_Key, 4, 0);
	KeyAddition(CipherTextTest, Expanded_Key, 8, 0);
	KeyAddition(CipherTextTest, Expanded_Key, 12, 0);

	for(int i = 0; i < 16; i++)
	{
		cout << CipherTextTest[i] << " ";
	}
	cout << endl;

	delete Expanded_Key;

	return(0);
}

void KeyAddition(uint32_t CipherText[], GF256* Expanded_Key, int start_Plain_Text, int start_key)
{
	uint32_t Key;

	for(int i = 0; i < 4; i++)
	{
		Key = 0;
		for(int j = 0; j < 4; j++)
		{
			Key <<= 8;
			Key += Expanded_Key[start_key+4*i+j].to_int();
		}
		CipherText[start_Plain_Text+i] ^= Key;
	}

	return;
}

GF256* Key_Expansion(uint8_t Key[])
{
	int i;
	GF256* w;
	GF256 temp[4];
	GF256 rcon[10][4];
	w = new GF256[16*(Nr+1)];

	for(i = 0; i < 4; i++)
		rcon[0][i] = GF256(0);
	rcon[0][0] = GF256(1);
	for(i = 1; i < 10; i++)
		for(int j = 0; j < 4; j++)
			rcon[i][j] = rcon[i-1][j]*2;

	for(i = 0; i < 32; i++)
		w[i] = Key[i];

	for(i = 8; i<=4*Nr+3; i++)
	{
		temp[0] = w[4*(i-1)];
		temp[1] = w[4*(i-1)+1];
		temp[2] = w[4*(i-1)+2];
		temp[3] = w[4*(i-1)+3];

		if(i%Nk == 0)
		{
			Rotate_Word(temp,1);
			Sub_Word(temp);
			w[4*i] = w[4*i-32]+temp[0]+rcon[i/Nk][0];
			w[4*i+1] = w[4*i-31]+temp[1]+rcon[i/Nk][1];
			w[4*i+2] = w[4*i-30]+temp[2]+rcon[i/Nk][2];
			w[4*i+3] = w[4*i-29]+temp[3]+rcon[i/Nk][3];
		}
		else if(i%Nk == 4)
		{
			Sub_Word(temp);
			w[4*i] = w[4*i-32]+temp[0];
			w[4*i+1] = w[4*i-31]+temp[1];
			w[4*i+2] = w[4*i-30]+temp[2];
			w[4*i+3] = w[4*i-29]+temp[3];
		}
		else
		{
			w[4*i] = w[4*i-32];
			w[4*i+1] = w[4*i-31];
			w[4*i+2] = w[4*i-30];
			w[4*i+3] = w[4*i-29];
		}
	}

	return(w);
}

void Rotate_Word(GF256 * Word, int Shift)	//Useful for ShiftRows() and RotWord() in specification. Shift to the left Shift elements. Word had better be 4 bytes or there will be problems.
{
	GF256 temp[2];

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

void Sub_Word(GF256 * Word)	//Substitute from the SBox for 4 bytes
{
	for(int i = 0; i < 4; i++)
		Word[i] = SBox[Word[i].to_int()];
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

void Inv_Sub_Word(GF256 * Word)	//Substitute from the SBox for 4 bytes
{
	for(int i = 0; i < 4; i++)
		Word[i] = InvSBox[Word[i].to_int()];
}

GF256 circularLeftShift(GF256 num, int shift)
{
	return((num << shift) | (num >> (8 - shift)));
}

void Construct_SBox()
{
	GF256 p = 1, q = 1;
	
	do
	{
		//Multiply by x+1%(x^8+x^4+x^3+x+1) in GF(256), x+1 is 3 and coprime with 256. (p*=GF256(0x03))
		p = p*0x03;

		//Divide by x+1%(x^8+x^4+x^3+x+1), which is equivalent to multiplying by x^7+x^6+x^5+x^4+x^2+x%(x^8+x^4+x^3+x+1) in GF(256),  (q*=GF256(0xf6))
		q = q*0xF6;

		//Compute the affine transformation
		SBox[p.to_int()] = q ^ circularLeftShift(q, 1) ^ circularLeftShift(q, 2) ^ circularLeftShift(q, 3) ^ circularLeftShift(q, 4) ^ 0x63;
	}while(p != 1);

	//0 is a special case since it has no inverse
	SBox[0] = GF256(0x63);
}

void Construct_InvSBox()
{
	for(int i = 0; i < 256; i++)
		InvSBox[SBox[i].to_int()] = i;
}
