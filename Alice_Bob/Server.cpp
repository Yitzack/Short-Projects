#include<iostream>
#include<fstream>
#include<cstring>
#include<boost/asio.hpp>
#include"RSA.h"
#include"AES.h"
#include"SHA256.h"
#include"Hamming.h"
using namespace std;
using namespace boost::asio;
using ip::tcp;

void Initalize_RSA();
void Hexer(char*, cpp_int);

RSA RSA_Encryption;
SHA256 Hashing;

int main()
{
	Initalize_RSA();
	io_service io;	//I/O Context
	tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 50624));	//Create a connection acceptor into the I/O context

	//while(true)
	{
		tcp::socket socket(io);	//Creat a socket in the I/O Context
		acceptor.accept(socket);	//Accept a connection on the socket, passively wait for a connection

		uint8_t message[] = "Hello, client!";		//Before the client can send a message, send it a hello, it is looking for the hello
		boost::system::error_code ignored_error;	//Error Code that should be used in sending and receiving data. The error code can't be used unless a flag is also set.
		socket.send(buffer(message));	//Send the message

		uint8_t buf[1024];	//I don't know why a chacter array (either the primary datatype or std) has to be used to receive but a string can used to send.
		socket.receive(buffer(buf,1024));	//Receive the reply
		cout << "Received: " << buf << endl;

		// Check the client's IP address and ignore messages from other sources if needed
		tcp::endpoint remote_endpoint = socket.remote_endpoint();	//Record the socket remote endpoint
		string client_ip = remote_endpoint.address().to_string();	//Pull out the IP address of the remote endpoint
		cout << "Message received from IP address: " << client_ip << endl;	//Log the IP address
	}

	return(0);
}


void Initalize_RSA()
{
	ifstream iRSA_Keys("./RSA_Keys");
	ofstream oRSA_Keys;

	if(iRSA_Keys.is_open())
	{
		char dstr1[1101], estr1[101], nstr1[1101];
		iRSA_Keys.getline(dstr1, 1101);
		iRSA_Keys.getline(dstr1, 1101);
		iRSA_Keys.getline(estr1, 101);
		iRSA_Keys.getline(nstr1, 1101);
		iRSA_Keys.close();
		char dstr[1104] = "0x";
		char estr[1104] = "0x";
		char nstr[1104] = "0x";
		cpp_int d(strcat(dstr,dstr1));
		cpp_int e(strcat(estr,estr1));
		cpp_int n(strcat(nstr,nstr1));
		if(d <= 5)
			return;
		try
		{
			RSA_Encryption.set_Keys(d,e,n);
		}
		catch(const RSAException& Error)
		{
			if(Error.errorType == RSAException::ErrorType::MismatchKeys)
				goto Redo_RSA;
			else
				throw;
		}
		return;
	}

	Redo_RSA:
	RSA_Encryption.initalize();
	iRSA_Keys.close();
	oRSA_Keys.open("./RSA_Keys");
	oRSA_Keys << hex;
	RSA_Encryption.Print_Private_Key(oRSA_Keys);
	oRSA_Keys << RSA_Encryption.Public_key_e() << endl;
	oRSA_Keys << RSA_Encryption.Public_key_n();
	oRSA_Keys.close();

	oRSA_Keys.open("./Certificate");
	char Cert[3500] = "Server Public Key: ";
	char Hex_Code[1000];
	Hexer(Hex_Code, RSA_Encryption.Public_key_e());
	strcat(Cert, Hex_Code);
	strcat(Cert, ",");
	Hexer(Hex_Code, RSA_Encryption.Public_key_n());
	strcat(Cert, Hex_Code);
	strcat(Cert, "\nServer IP: 127.0.0.1\nServer Location: Author's Bedroom\nServer Organization: Independent\nCertificate Validity Period: Nov 30, 2024\nCA Name: Author\nCA Public Key: ");
	Hexer(Hex_Code, RSA_Encryption.Public_key_e());
	strcat(Cert, Hex_Code);
	strcat(Cert, ",");
	Hexer(Hex_Code, RSA_Encryption.Public_key_n());
	strcat(Cert, Hex_Code);
	strcat(Cert, "\nCertificate Serial Number: 0\n");
	uint32_t Hash[8];
	Hashing.Hash_func(Cert, strlen(Cert), Hash);
	oRSA_Keys << Cert << "Signature: " << RSA_Encryption.Encrypt(Hash, 8) << endl;
	oRSA_Keys.close();
}

void Hexer(char* Hex_Code, cpp_int Number) {
	const char hexDigits[] = "0123456789abcdef";
	int Length = 0;
	cpp_int temp = Number;

	while(temp > 0)	//Count the number of hex digits needed
	{
		temp >>= 4;
		Length++;
	}

	if(Length == 0)
	{
		Hex_Code[0] = '0';	//If the number is zero, the hexadecimal representation is "0"
		Hex_Code[1] = '\0';
		return;
	}

	// Fill Hex_Code from right to left
	for (int i = Length - 1; i >= 0; i--)
	{
		int digit = int((Number >> (4 * i)) & 0xF);	//Extract 4 bits at a time
		Hex_Code[Length - i - 1] = hexDigits[digit];
	}

	Hex_Code[Length] = '\0';	//Null-terminate the string
}






