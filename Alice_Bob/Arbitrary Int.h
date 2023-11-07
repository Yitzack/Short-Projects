//This is my implementation of an arbitrary precision integer library
#include<vector>
using namespace std;

#ifndef ARBIT_INT
#define ARBIT_INT

class Arbit_int
{
	public:
		Arbit_int();
		Arbit_int(vector<bool>);
		Arbit_int(int);
		Arbit_int(unsigned int);
		Arbit_int(unsigned long int);
		Arbit_int(unsigned long long int);
		Arbit_int operator=(Arbit_int);
		Arbit_int operator<<=(int);
		Arbit_int operator>>=(int);
		Arbit_int operator<<(int);
		Arbit_int operator>>(int);
		Arbit_int operator+(Arbit_int);
		Arbit_int operator+(int);
		Arbit_int operator-(Arbit_int);
		Arbit_int operator-(int);
		Arbit_int operator*(Arbit_int);
		Arbit_int operator/(Arbit_int);
		Arbit_int operator%(Arbit_int);
		Arbit_int operator%(int);
		bool operator==(Arbit_int);
		bool operator==(int);
		bool operator!=(Arbit_int);
		bool operator!=(int);
		bool operator>(Arbit_int);
		bool operator>(int);
		bool operator<(Arbit_int);
		bool operator<(int);
		bool operator=>(Arbit_int);
		bool operator=>(int);
		bool operator=<(Arbit_int);
		bool operator=<(int);
	private:
		vector<bool> Data;
};

#endif

