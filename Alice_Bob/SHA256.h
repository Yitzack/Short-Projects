#include<iostream>
#include<cstring>
#include<cstdint>	//defines data type uintN_t for unsigned N-bit integers
using namespace std;

class SHA256
{
	public:
		SHA256(){};
		void Hash_func(uint8_t*, uint64_t Length, uint32_t*);
		void Hash_func(char*, uint64_t Length, uint32_t*);
	private:
		uint32_t RightRot(uint32_t, int);
		uint32_t Ch(uint32_t, uint32_t, uint32_t);
		uint32_t Maj(uint32_t, uint32_t, uint32_t);
		uint32_t Sigma0(uint32_t);
		uint32_t Sigma1(uint32_t);
		uint32_t sigma0(uint32_t);
		uint32_t sigma1(uint32_t);
		void DataCopy(uint32_t[], uint8_t*);
		uint32_t h0[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
		uint32_t k[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
};

uint32_t SHA256::RightRot(uint32_t Word, int i)
{
	return((Word >> i) | (Word << (32 - i)));
}

uint32_t SHA256::Ch(uint32_t x, uint32_t y, uint32_t z)
{
	return((x&y)^((~x)&z));
}

uint32_t SHA256::Maj(uint32_t x, uint32_t y, uint32_t z)
{
	return((x&y)^(x&z)^(y&z));
}

uint32_t SHA256::Sigma0(uint32_t x)
{
	return(RightRot(x,2)^RightRot(x,13)^RightRot(x,22));
}

uint32_t SHA256::Sigma1(uint32_t x)
{
	return(RightRot(x,6)^RightRot(x,11)^RightRot(x,25));
}

uint32_t SHA256::sigma0(uint32_t x)
{
	return(RightRot(x,7)^RightRot(x,18)^(x>>3));
}

uint32_t SHA256::sigma1(uint32_t x)
{
	return(RightRot(x,17)^RightRot(x,19)^(x>>10));
}

void SHA256::DataCopy(uint32_t w[], uint8_t* Message)
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


void SHA256::Hash_func(char* Message, uint64_t Length, uint32_t* Hash)
{
	int Blocks = ((Length<<3)+64)/512+1;
	uint8_t* Mess = new uint8_t[Blocks*64];
	memcpy(Mess, Message, Length);
	Hash_func(Mess, Length, Hash);
}

void SHA256::Hash_func(uint8_t* Message, uint64_t Length, uint32_t* Hash)
{
	int Blocks;
	int i, j;
	uint32_t w[64], a,b,c,d,e,f,g,h;

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

		a = h0[0];
		b = h0[1];
		c = h0[2];
		d = h0[3];
		e = h0[4];
		f = h0[5];
		g = h0[6];
		h = h0[7];

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

		h0[0] = h0[0] + a;
		h0[1] = h0[1] + b;
		h0[2] = h0[2] + c;
		h0[3] = h0[3] + d;
		h0[4] = h0[4] + e;
		h0[5] = h0[5] + f;
		h0[6] = h0[6] + g;
		h0[7] = h0[7] + h;
	}

	for(i = 0; i < 8; i++)
		Hash[i] = h0[i];
}
