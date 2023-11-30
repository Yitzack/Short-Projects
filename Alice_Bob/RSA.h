#include<iostream>
#include<cmath>
#include<random>
#include<limits>
#include<exception>
#include<string>
#include<boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost::multiprecision;

#ifndef RSA_HEADER
#define RSA_HEADER

class RSAException : public exception
{
	public:
		enum class ErrorType	//Enumeration of error types
		{
			MessageTooLarge,
			CodeTooLarge,
			MismatchKeys
		};

		ErrorType errorType;

		RSAException(ErrorType type) : errorType(type)	//Constructor of the RSAException
		{
			switch(type)	//Setting the error message based off the error type
			{
				case ErrorType::MessageTooLarge:
					errorMessage = "Message is greater than common key modulo";
					break;
				case ErrorType::CodeTooLarge:
					errorMessage = "Code is greater than common key modulo";
					break;
				case ErrorType::MismatchKeys:
					errorMessage = "Private and public keys are not a pair";
					break;
			}
		}

		virtual const char* what() const throw()	//Allow the error message to be retrived in a try/catch block
		{
			return(errorMessage.c_str());
		}
	private:
		string errorMessage;
};

class RSA
{
	public:
		RSA();					//Do nothing constructor
		RSA(cpp_int, cpp_int);			//Set the public key of the RSA object
		RSA(cpp_int, cpp_int, cpp_int);	//Set the private and public key of the RSA object
		~RSA(){};				//Destructor
		RSA(const RSA& other);			//Copy constructor
		RSA(RSA&& other) noexcept;		//Move constructor
		RSA& operator=(const RSA& other);	//Copy assignment operator
		RSA& operator=(RSA&& other) noexcept;	//Move assignment operator
		cpp_int Public_key_n() const;		//Output the public key n=pq
		cpp_int Public_key_e() const;		//Output the public key e such that (e*d)%lamda(n)=1
		cpp_int Encrypt(uint8_t[], int) const;//Encrypt with the message with the public key
		cpp_int Encrypt(uint32_t[], int) const;//Encrypt with the message with the public key
		cpp_int Encrypt(cpp_int) const;	//Encrypt with the message with the public key
		cpp_int Sign(uint8_t[], int) const;	//Sign the message, that is encrypt with the message with the private key
		cpp_int Sign(uint32_t[], int) const;	//Sign the message, that is encrypt with the message with the private key
		cpp_int Sign(cpp_int) const;		//Sign the message, that is encrypt with the message with the private key
		cpp_int Decrypt(cpp_int) const;	//Decrypt with the message with the private key
		void set_Public_key_n(cpp_int);	//Set the public key n=pq
		void set_Public_key_n(uint8_t[]);	//Set the public key n=pq
		void set_Public_key_e(cpp_int);	//Set the public key e such that (e*d)%lambda(n)=1
		void set_Public_key_e(uint8_t[]);	//Set the public key e such that (e*d)%lambda(n)=1
		void set_Keys(cpp_int, cpp_int, cpp_int);
		void initalize();			//Initalize the public and private key pairs
		void Print_Private_Key(ostream&) const;//Print the private key, under no circumstance shall this function provide the private key in program. It shall only go out of program to file, stdout, or stderr. The user will have to release the private key themselves directly or load it back in themselves.
	private:
		cpp_int private_key_d;
		cpp_int key_n;
		cpp_int public_key_e;
		bool AKS_Prime_Test(cpp_int) const;		//AKS primaility test (not used in the current implemenation because it takes forever)
		bool prime_test(cpp_int) const;		//Probablistic prime test, uses the first 100 primes as witness numbers. Primes are particularlly good at the task.
		cpp_int Carmichael(cpp_int, cpp_int) const;	//Carmichael Totient Function of p*q (lcm(p-1, q-1)=(q-1)(p-1)/(gcd(p-1, q-1)))
		cpp_int Euclidean(cpp_int, cpp_int) const;	//Extended Euclidean algorithm to solve 1=(d*e)%lambda
		cpp_int GCD(cpp_int, cpp_int) const;		//Greatest common denominator
		cpp_int EulerTotient(cpp_int) const;		//Euler Totient Function
		cpp_int PowMod(cpp_int b, cpp_int n, cpp_int m) const;	//returns (b^n)%m
		cpp_int pow(cpp_int b, cpp_int n) const;	//return b^n
};
#endif

