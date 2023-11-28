#include<iostream>
#include<iomanip>
#include<random>
#include<cstdint>	//defines data type uint8_t for 1-byte (8-bit) integers
#include<cstring>
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
		~AES(){};				//Destructor
		AES(const AES& other);			//Copy constructor
		AES(AES&& other) noexcept;		//Move constructor
		AES& operator=(const AES& other);	//Copy assignment operator
		AES& operator=(AES&& other) noexcept;	//Move assignment operator
		void Encrypt(GF256[], GF256*, int) const;
		void Decrypt(GF256[], GF256*, int) const;
		void Encrypt(uint8_t[], GF256*, int) const;
		void Decrypt(uint8_t[], GF256*, int) const;
		void Encrypt(uint32_t[], GF256*, int) const;
		void Decrypt(uint32_t[], GF256*, int) const;
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
		GF256 circularLeftShift(GF256, int) const;
		void Rotate_Word(GF256 *, int) const;
		void Sub_Word(GF256 *) const;
		void Round(GF256[]) const;
		void Inv_Round(GF256[],int) const;
		void Partial_Round(GF256[]) const;
		void Inv_Partial_Round(GF256[]) const;
		void Inv_Rotate_Word(GF256 *, int) const;
		void Inv_Sub_Word(GF256 *) const;
		void KeyAddition(GF256[], const GF256*, int) const;
		void Output(GF256*, const char[]) const;
};

#endif

