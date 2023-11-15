#include<iostream>
#include<iomanip>
#include<random>
#include<cstdint>	//defines data type uint8_t for 1-byte (8-bit) integers
#include"GF256.h"
using namespace std;

#ifndef AES_HEADER
#define AES_HEADER

class AES
{
	public:
		AES();
		AES(uint8_t[]);
		AES(GF256[]);
		void Encrypt(GF256[], GF256*, int);
		void Decrypt(GF256[], GF256*, int);
		void Encrypt(uint8_t[], GF256*, int);
		void Decrypt(uint8_t[], GF256*, int);
		void Encrypt(uint32_t[], GF256*, int);
		void Decrypt(uint32_t[], GF256*, int);
		void Set_Key(uint8_t[]);
		void Set_Key(GF256[]);
	private:
		GF256 SBox[256];
		GF256 InvSBox[256];
		GF256 Expanded_Key[240];
		const int Nk = 8;	//Number of 4-byte words that make up the AES-256 key.
		const int Nr = 14;	//Number of rounds taken in AES-256
		void Construct_SBox();
		void Construct_InvSBox();
		void Key_Expansion(uint8_t[]);
		GF256 circularLeftShift(GF256, int);
		void Rotate_Word(GF256 *, int);
		void Sub_Word(GF256 *);
		void Round(GF256[]);
		void Inv_Round(GF256[],int);
		void Partial_Round(GF256[]);
		void Inv_Partial_Round(GF256[]);
		void Inv_Rotate_Word(GF256 *, int);
		void Inv_Sub_Word(GF256 *);
		void KeyAddition(GF256[], GF256*, int);
};

#endif

AES::AES()
{
	Construct_SBox();
	Construct_InvSBox();
}

AES::AES(uint8_t Key[])
{
	Construct_SBox();
	Construct_InvSBox();
	Key_Expansion(Key);
}

AES::AES(GF256 Key[])
{
	uint8_t IntKey[32];
	for(int i = 0; i < 32; i++)
		IntKey[i] = Key[i].to_int();
	Construct_SBox();
	Construct_InvSBox();
	Key_Expansion(IntKey);
}


void AES::Set_Key(uint8_t Key[])
{
	Key_Expansion(Key);
}

void AES::Set_Key(GF256 Key[])
{
	uint8_t IntKey[32];
	for(int i = 0; i < 32; i++)
		IntKey[i] = Key[i].to_int();
	Key_Expansion(IntKey);
}

void AES::Decrypt(uint8_t CipherText8[], GF256* PlainText, int length)
{
	int Blocks = length/16;	//How long is the plaintext
	if(length%16 != 0)
		Blocks++;

	GF256 CipherText[16*Blocks];
	for(int i = 0; i < length; i++)
		CipherText[i] = CipherText8[i];
	Decrypt(CipherText, PlainText, length);
}

void AES::Encrypt(uint8_t PlainText8[], GF256* CipherText, int length)
{
	int Blocks = length/16;	//How long is the plaintext
	if(length%16 != 0)
		Blocks++;

	GF256 PlainText[16*Blocks];
	for(int i = 0; i < length; i++)
		PlainText[i] = PlainText8[i];
	Encrypt(PlainText, CipherText, length);
}

void AES::Decrypt(uint32_t CipherText32[], GF256* PlainText, int length)
{
	int Blocks = length/16;	//How long is the plaintext
	if(length%16 != 0)
		Blocks++;

	GF256 CipherText[16*Blocks];
	for(int i = 0; i < 16; i++)
	{
		CipherText[4*i] = CipherText32[i] >> 24;
		CipherText[4*i+1] = (CipherText32[i] >> 16) & 0xFF;
		CipherText[4*i+2] = (CipherText32[i] >> 8) & 0xFF;
		CipherText[4*i+3] = CipherText32[i] & 0xFF;
	}
	Encrypt(CipherText, PlainText, length);
}

void AES::Encrypt(uint32_t PlainText32[], GF256* CipherText, int length)
{
	int Blocks = length/16;	//How long is the plaintext
	if(length%16 != 0)
		Blocks++;

	GF256 PlainText[16*Blocks];
	for(int i = 0; i < 16; i++)
	{
		PlainText[4*i] = PlainText32[i] >> 24;
		PlainText[4*i+1] = (PlainText32[i] >> 16) & 0xFF;
		PlainText[4*i+2] = (PlainText32[i] >> 8) & 0xFF;
		PlainText[4*i+3] = PlainText32[i] & 0xFF;
	}
	Encrypt(PlainText, CipherText, length);
}

void AES::Decrypt(GF256 CipherText[], GF256* PlainText, int length)
{
	int Blocks = length/16;	//How long is the plaintext
	if(length%16 != 0)
		Blocks++;

	for(int i = 0; i < 16*Blocks; i++)	//Copy the CipherText into Text
		PlainText[i] = CipherText[i];

	for(int j = 0; j < Blocks; j += 16)	//Decrypt 16 bytes at a time
	{
		KeyAddition(&PlainText[j], &Expanded_Key[224], 0);
		for(int i = 13; i >= 1; i--)
		{
			Inv_Round(&PlainText[j],i);
		}
		Inv_Partial_Round(&PlainText[j]);
		KeyAddition(&PlainText[j], Expanded_Key, 0);
	}

	for(int i = length; i < 16*Blocks; i++)	//Change the last interval of PlainText to null characters or 0 depending on the data type
		PlainText[i] = 0;
}

void AES::Encrypt(GF256 PlainText[], GF256* CipherText, int length)
{
	int Blocks = length/16;	//How long is the plaintext
	int i,j;

	if(length%16 != 0)
		Blocks++;

	for(i = 0; i < length; i++)	//Copy the CipherText into Text
		CipherText[i] = PlainText[i];
	for(; i < 16*Blocks; i++)
		CipherText[i] = 0;	//Pad out the rest of the PlainText with null characters or 0

	for(j = 0; j < 16*Blocks; j += 4)
	{
		KeyAddition(&CipherText[j], Expanded_Key, 0);	//Encrypt 16 bytes at a time
		for(int i = 1; i <= 13; i++)
		{
			Round(&CipherText[j]);
			KeyAddition(&CipherText[j], &Expanded_Key[16*i], 0);
		}
		Partial_Round(&CipherText[j]);
		KeyAddition(&CipherText[j], &Expanded_Key[224], 0);
	}
}

void AES::Inv_Partial_Round(GF256 Text[])
{
	GF256 Block[4][4];
	int i,j;

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			Block[i][j] = Text[4*j+i];

	for(i = 0; i < 4; i++)	//Substitution and row shifting (commutible)
	{
		Inv_Sub_Word(Block[i]);
		Inv_Rotate_Word(Block[i], i);
	}

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			Text[4*j+i] = Block[i][j];

	return;
}

void AES::Partial_Round(GF256 Text[])
{
	GF256 Block[4][4];
	GF256 M[4][4] = {{GF256(2),GF256(3),GF256(1),GF256(1)},
			  {GF256(1),GF256(2),GF256(3),GF256(1)},
			  {GF256(1),GF256(1),GF256(2),GF256(3)},
			  {GF256(3),GF256(1),GF256(1),GF256(2)}};
	int i,j,k;

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			Block[i][j] = Text[4*j+i];

	for(i = 0; i < 4; i++)	//Substitution and row shifting (commutible)
	{
		Sub_Word(Block[i]);
		Rotate_Word(Block[i], i);
	}

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			Text[4*j+i] = Block[i][j];

	return;
}

void AES::Inv_Round(GF256 Text[], int Round)
{
	GF256 Block[4][4];
	GF256 Block1[4][4];
	GF256 M[4][4] = {{GF256(14),GF256(11),GF256(13),GF256(9)},
			  {GF256(9),GF256(14),GF256(11),GF256(13)},
			  {GF256(13),GF256(9),GF256(14),GF256(11)},
			  {GF256(11),GF256(13),GF256(9),GF256(14)}};
	int i,j,k;

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			Block1[i][j] = Text[4*j+i];

	for(i = 0; i < 4; i++)	//Substitution and row shifting (commutible)
	{
		Inv_Sub_Word(Block1[i]);
		Inv_Rotate_Word(Block1[i], i);
	}

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			Text[4*j+i] = Block1[i][j];

	KeyAddition(Text, &Expanded_Key[16*Round], 0); 

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			Block1[i][j] = Text[4*j+i];

	for(i = 0; i < 4; i++)	//4x4 times 4x4 matrix multiplication
		for(j = 0; j < 4; j++)
			for(k = 0; k < 4; k++)
				Block[k][i] = Block[k][i] + M[k][j]*Block1[j][i];

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			Text[4*j+i] = Block[i][j];

	return;
}

void AES::Round(GF256 Text[])
{
	GF256 Block[4][4];
	GF256 M[4][4] = {{GF256(2),GF256(3),GF256(1),GF256(1)},
			  {GF256(1),GF256(2),GF256(3),GF256(1)},
			  {GF256(1),GF256(1),GF256(2),GF256(3)},
			  {GF256(3),GF256(1),GF256(1),GF256(2)}};
	int i,j,k;

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			Block[i][j] = Text[4*j+i];

	for(i = 0; i < 4; i++)	//Substitution and row shifting (commutible)
	{
		Sub_Word(Block[i]);
		Rotate_Word(Block[i], i);
	}

	for(i = 0; i < 16; i++)
		Text[i] = 0;

	for(i = 0; i < 4; i++)	//4x4 times 4x4 matrix multiplication
		for(j = 0; j < 4; j++)
			for(k = 0; k < 4; k++)
				Text[4*i+k] = Text[4*i+k] + M[k][j]*Block[j][i];

	return;
}

void AES::KeyAddition(GF256 Text[], GF256* Expanded_Key, int start_Text)
{
	for(int i = 0; i < 16; i++)
	{
		Text[start_Text+i] = Text[start_Text+i] ^ Expanded_Key[i];
	}

	return;
}

void AES::Key_Expansion(uint8_t Key[])
{
	int i;
	GF256 temp[4];
	GF256 rcon[10][4];

	for(i = 0; i < 4; i++)
		rcon[0][i] = GF256(0);
	rcon[0][0] = GF256(1);
	for(i = 1; i < 10; i++)
		for(int j = 0; j < 4; j++)
			rcon[i][j] = rcon[i-1][j]*2;

	for(i = 0; i < 32; i++)
		Expanded_Key[i] = Key[i];

	for(i = 8; i<=4*Nr+3; i++)
	{
		temp[0] = Expanded_Key[4*(i-1)];
		temp[1] = Expanded_Key[4*(i-1)+1];
		temp[2] = Expanded_Key[4*(i-1)+2];
		temp[3] = Expanded_Key[4*(i-1)+3];

		if(i%Nk == 0)
		{
			Rotate_Word(temp,1);
			Sub_Word(temp);
			Expanded_Key[4*i] = Expanded_Key[4*i-32]+temp[0]+rcon[i/Nk-1][0];
			Expanded_Key[4*i+1] = Expanded_Key[4*i-31]+temp[1]+rcon[i/Nk-1][1];
			Expanded_Key[4*i+2] = Expanded_Key[4*i-30]+temp[2]+rcon[i/Nk-1][2];
			Expanded_Key[4*i+3] = Expanded_Key[4*i-29]+temp[3]+rcon[i/Nk-1][3];
		}
		else if(i%Nk == 4)
		{
			Sub_Word(temp);
			Expanded_Key[4*i] = Expanded_Key[4*i-32]+temp[0];
			Expanded_Key[4*i+1] = Expanded_Key[4*i-31]+temp[1];
			Expanded_Key[4*i+2] = Expanded_Key[4*i-30]+temp[2];
			Expanded_Key[4*i+3] = Expanded_Key[4*i-29]+temp[3];
		}
		else
		{
			Expanded_Key[4*i] = Expanded_Key[4*i-32]+temp[0];
			Expanded_Key[4*i+1] = Expanded_Key[4*i-31]+temp[1];
			Expanded_Key[4*i+2] = Expanded_Key[4*i-30]+temp[2];
			Expanded_Key[4*i+3] = Expanded_Key[4*i-29]+temp[3];
		}
	}
}

void AES::Rotate_Word(GF256 * Word, int Shift)	//Useful for ShiftRows() and RotWord() in specification. Shift to the left Shift elements. Word had better be 4 bytes or there will be problems.
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

void AES::Sub_Word(GF256 * Word)	//Substitute from the SBox for 4 bytes
{
	for(int i = 0; i < 4; i++)
		Word[i] = SBox[Word[i].to_int()];
}

void AES::Inv_Rotate_Word(GF256 * Word, int Shift)	//Useful for ShiftRows() and RotWord() in specification. Shift to the left Shift elements. Word had better be 4 bytes or there will be problems.
{
	GF256 temp[2];

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

void AES::Inv_Sub_Word(GF256 * Word)	//Substitute from the SBox for 4 bytes
{
	for(int i = 0; i < 4; i++)
		Word[i] = InvSBox[Word[i].to_int()];
}

GF256 AES::circularLeftShift(GF256 num, int shift)
{
	return((num << shift) | (num >> (8 - shift)));
}

void AES::Construct_SBox()
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

void AES::Construct_InvSBox()
{
	for(int i = 0; i < 256; i++)
		InvSBox[SBox[i].to_int()] = i;
}
