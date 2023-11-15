#include<iostream>
using namespace std;

#ifndef GF256_HEADER
#define GF256_HEADER

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

#endif

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

