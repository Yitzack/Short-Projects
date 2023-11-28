#include<iostream>
#include<cstring>
#include<cstdint>	//defines data type uintN_t for unsigned N-bit integers
#include"SHA256.h"
using namespace std;

uint32_t SHA256::RightRot(uint32_t Word, int i) const
{
	return((Word >> i) | (Word << (32 - i)));
}

uint32_t SHA256::Ch(uint32_t x, uint32_t y, uint32_t z) const
{
	return((x&y)^((~x)&z));
}

uint32_t SHA256::Maj(uint32_t x, uint32_t y, uint32_t z) const
{
	return((x&y)^(x&z)^(y&z));
}

uint32_t SHA256::Sigma0(uint32_t x) const
{
	return(RightRot(x,2)^RightRot(x,13)^RightRot(x,22));
}

uint32_t SHA256::Sigma1(uint32_t x) const
{
	return(RightRot(x,6)^RightRot(x,11)^RightRot(x,25));
}

uint32_t SHA256::sigma0(uint32_t x) const
{
	return(RightRot(x,7)^RightRot(x,18)^(x>>3));
}

uint32_t SHA256::sigma1(uint32_t x) const
{
	return(RightRot(x,17)^RightRot(x,19)^(x>>10));
}

void SHA256::DataCopy(uint32_t w[], uint8_t* Message) const
{
	for(int i = 0; i < 16; i++)
	{
		w[i] = Message[4*i] << 24;
		w[i] += Message[4*i+1] << 16;
		w[i] += Message[4*i+2] << 8;
		w[i] += Message[4*i+3];
	}
	return;
}


void SHA256::Hash_func(char* Message, uint64_t Length, uint32_t* Hash) const
{
	int Blocks = ((Length<<3)+64)/512+1;
	uint8_t* Mess = new uint8_t[Blocks*64];
	memcpy(Mess, Message, Length);
	Hash_func(Mess, Length, Hash);
	delete Mess;
}

void SHA256::Hash_func(uint8_t* Message, uint64_t Length, uint32_t* Hash) const
{
	int Blocks;
	int i, j;
	uint32_t w[64], a,b,c,d,e,f,g,h;
	uint32_t h1[8];
	memcpy(h1, h0, 32);

	Blocks = ((Length<<3)+64)/512+1;
	Message[Length] = 0x80;
	for(i = Length+1; i < 64*Blocks-8; i++)
		Message[i] = 0;
	Length <<= 3;
	for(i = 64*Blocks-1; i >= 64*Blocks-8; i--)
	{
		Message[i] = Length & 0xFF;
		Length >>= 8;
	}

	for(i = 0; i < Blocks; i++)
	{
		DataCopy(w, &Message[i*64]);

		for(j = 16; j < 64; j++)
			w[j] = sigma1(w[j-2])+w[j-7]+sigma0(w[j-15])+w[j-16];

		a = h1[0];
		b = h1[1];
		c = h1[2];
		d = h1[3];
		e = h1[4];
		f = h1[5];
		g = h1[6];
		h = h1[7];

		for(j = 0; j < 64; j++)
		{
			uint32_t temp1 = h+Sigma1(e)+Ch(e,f,g)+k[j]+w[j];
			uint32_t temp2 = Sigma0(a)+Maj(a,b,c);

			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}

		h1[0] = h1[0] + a;
		h1[1] = h1[1] + b;
		h1[2] = h1[2] + c;
		h1[3] = h1[3] + d;
		h1[4] = h1[4] + e;
		h1[5] = h1[5] + f;
		h1[6] = h1[6] + g;
		h1[7] = h1[7] + h;
	}

	for(i = 0; i < 8; i++)
		Hash[i] = h1[i];
}
