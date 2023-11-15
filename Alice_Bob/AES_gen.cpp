#include<iostream>
#include<iomanip>
#include<random>
#include<cstdint>	//defines data type uint8_t for 1-byte (8-bit) integers
#include"AES.h"
using namespace std;

int main()
{
	uint8_t Key[] = {0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81, 0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4};	//32-block key of 8-bit blocks
	uint32_t PlainText[] = {0x6BC1BEE2, 0x2E409F96, 0xE93D7E11, 0x7393172A, 0xAE2D8A57, 0x1E03AC9C, 0x9EB76FAC, 0x45AF8E51, 0x30C81C46, 0xA35CE411, 0xE5FBC119, 0x1A0A52EF, 0xF69F2445, 0xDF4F9B17, 0xAD2B417B, 0xE66C3710};	//Plain text to test with
	uint32_t CipherTextCorrect[] = {0xF3EED1BD, 0xB5D2A03C, 0x064B5A7E, 0x3DB181F8, 0x591CCB10, 0xD410ED26, 0xDC5BA74A, 0x31362870, 0xB6ED21B9, 0x9CA6F4F9, 0xF153E7B1, 0xBEAFED1D, 0x23304B7A, 0x39F9F3FF, 0x067D8D8F, 0x9E24ECC7};	//Correct cipher text
	uint32_t CipherTextTest[16];	//Place to store result of encyrption
	uint32_t PlainTextTest[16];	//Place to store result of decryption
	GF256 IntermediateText[64];
	AES Code(Key);

	cout << hex;

	Code.Encrypt(PlainText, IntermediateText, 64);

	for(int i = 0; i < 16; i++)
	{
		CipherTextTest[i] = IntermediateText[4*i].to_int() << 24;
		CipherTextTest[i] += IntermediateText[4*i+1].to_int() << 16;
		CipherTextTest[i] += IntermediateText[4*i+2].to_int() << 8;
		CipherTextTest[i] += IntermediateText[4*i+3].to_int();
		cout << CipherTextTest[i] << " ";
	}
	cout << endl;
	for(int i = 0; i < 16; i++)
	{
		cout << CipherTextCorrect[i] << " ";
	}
	cout << endl;

	Code.Decrypt(CipherTextTest, IntermediateText, 64);

	for(int i = 0; i < 16; i++)
	{
		PlainTextTest[i] = IntermediateText[4*i].to_int() << 24;
		PlainTextTest[i] += IntermediateText[4*i+1].to_int() << 16;
		PlainTextTest[i] += IntermediateText[4*i+2].to_int() << 8;
		PlainTextTest[i] += IntermediateText[4*i+3].to_int();
		cout << PlainTextTest[i] << " ";
	}
	cout << endl;
	for(int i = 0; i < 16; i++)
	{
		cout << PlainText[i] << " ";
	}
	cout << endl;

	return(0);
}
