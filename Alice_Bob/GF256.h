#include<iostream>
#include<cstdint>	//defines data type uint8_t for 1-byte (8-bit) integers
using namespace std;

#ifndef GF256_HEADER
#define GF256_HEADER

class GF256	// Galois Field(256) implementation
{
	public:
		GF256();
		GF256(uint8_t);
		uint8_t to_int() const;
		GF256 operator=(GF256);
		GF256 operator=(uint8_t);
		bool operator==(GF256) const;
		bool operator==(uint8_t) const;
		bool operator!=(GF256) const;
		bool operator!=(uint8_t) const;
		GF256 operator+(GF256) const;
		GF256 operator+(uint8_t) const;
		GF256 operator-(GF256) const;
		GF256 operator-(uint8_t) const;
		GF256 operator*(GF256) const;
		GF256 operator*(uint8_t) const;
		GF256 operator^(GF256) const;
		GF256 operator^(uint8_t) const;
		GF256 operator|(GF256) const;
		GF256 operator|(uint8_t) const;
		GF256 operator&(GF256) const;
		GF256 operator&(uint8_t) const;
		//GF256 operator/(GF256) const; //I don't think I need this one.
		friend ostream& operator<<(ostream&, const GF256&);
		GF256 operator<<(uint8_t) const;
		GF256 operator>>(uint8_t) const;
	private:
		uint8_t Byte;
};

#endif
