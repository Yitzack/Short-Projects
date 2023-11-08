#include<iostream>
#include<boost/multiprecision/cpp_int.hpp>
#include"RSA.h"
using namespace std;
using namespace boost::multiprecision;

int main()
{
	RSA rsa;
	cpp_int Message;
	mt19937 RNG(time(NULL));
	uniform_int_distribution<unsigned long long int> Prime(0,4294967295);

	rsa.initalize();

	for(int i = 0; i < 126; i++)
	{
		Message += Prime(RNG);
		Message <<= 32;
	}
	Message += Prime(RNG);

	cpp_int Cypher = rsa.Encrypt(Message);
	cpp_int Decrypt = rsa.Decrypt(Cypher);

	cout << "Message:  " << Message << endl;
	cout << "Cypher:   " << Cypher << endl;
	cout << "Message': " << Decrypt << endl;
	if(Message == Decrypt)
		cout << "Success" << endl;
	else
		cout << "Error" << endl;

	cout << hex << rsa.Public_key_n() << " " << rsa.Public_key_e() << endl;
	rsa.Print_Private_Key(cout);

	return(0);
}
