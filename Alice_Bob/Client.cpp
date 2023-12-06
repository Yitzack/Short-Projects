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

RSA RSA_Encryption;
RSA Server_Encryption;
AES AES_Encryption;
SHA256 Hashing;

bool Auth_Server(cpp_int, cpp_int);	//Loads the server certificate and authenticates the server.
cpp_int Dehexer(char[], int, int);	//Changes a number stored as hexidecimal as characters into integer

int main()
{
	RSA_Encryption.initalize();
	io_service io;	//I/O Context
	tcp::socket socket(io);	//Creat a socket in the I/O Context
	socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 50624));	//Connect the socket to the server at 127.0.0.1:50624
	int i;

	uint8_t RSAServer[576];	//I don't know why a chacter array (either the primary datatype or std) has to be used to receive but a string can used to send.
	boost::system::error_code error;	//Error Code that should be used in sending and receiving data. The error code can't be used unless a flag is also set.
	socket.receive(buffer(RSAServer,576));	//Receive into the character array, a data stream from the socket and store it in the buffer.

	uint8_t RSAClient[576];	//Send a message back to the server
	Hamming Hmessage[9];

	cpp_int Number = RSA_Encryption.Public_key_n();	//Construct a message of the server's public keys and the port to communicate over
	for(i = 511; i >= 0; i--)
	{
		RSAClient[i] = uint8_t(Number & 0xFF);
		Number >>= 8;
	}
	Number = RSA_Encryption.Public_key_e();
	RSAClient[512] = uint8_t((Number & 0xFF0000) >> 16);
	RSAClient[513] = uint8_t((Number & 0xFF00) >> 8);
	RSAClient[514] = uint8_t(Number & 0xFF);
	for(i = 515; i < 576; i++)
		RSAClient[i] = 0;

	for(i = 0; i < 9; i++)	//Convert the message from uint8_t[] to Hamming[] and store in uint8_t[]
		Hmessage[i].Encode(&RSAClient[i*62]);
	for(i = 0; i < 9; i++)
	{
		Number = Hmessage[i].Export_Encoding();
		for(int j = 63; j >= 0; j--)
		{
			RSAClient[i*64+j] = uint8_t(Number & 0xff);
			Number >>= 8;
		}
	}
	socket.send(buffer(RSAClient,576));

	bool decode_success = true;
	for(i = 0; i < 9; i++)
	{
		decode_success &= Hmessage[i].Decode(&RSAClient[i*64]);
		Hmessage[i].Set_Encoding(&RSAServer[i*64]);
		decode_success &= Hmessage[i].Decode(&RSAServer[i*64]);
	}
	if(!decode_success)	//Message has been tampered with beyond recovery and Client will need to relaunch
	{
		cout << "Message has been altered beyond recovery. Please relaunch to try again." << endl;
		return(0);
	}
	int writeIndex = 0;
	for(i = 0; i < 576; i++)
		if(!(i % 64 == 62 || i % 64 == 63))
		{
			RSAClient[writeIndex] = RSAClient[i];
			RSAServer[writeIndex] = RSAServer[i];
			writeIndex++;
		}

	cout << "Client Sent: " << endl << hex;
	for(i = 0; i < 576; i++)
	{
		if(RSAClient[i] < 16)
			cout << 0;
		cout << uint16_t(RSAClient[i]);
		if(i%4 == 3)
			cout << " ";
		if(i%16 == 15)
			cout << "| ";
		if(i%64 == 63)
			cout << endl;
	}
	cout << "\nClient Received: " << endl << hex;
	for(i = 0; i < 576; i++)
	{
		if(RSAServer[i] < 16)
			cout << 0;
		cout << uint16_t(RSAServer[i]);
		if(i%4 == 3)
			cout << " ";
		if(i%16 == 15)
			cout << "| ";
		if(i%64 == 63)
			cout << endl;
	}
	cout << endl << dec;

	Server_Encryption.set_Public_key_n(RSAServer);
	Server_Encryption.set_Public_key_e(&RSAServer[512]);
	if(Auth_Server(Server_Encryption.Public_key_n(), Server_Encryption.Public_key_e()))
	{
		socket.close();
		socket.connect(tcp::endpoint(tcp::v4(), int(RSAServer[515] << 8)|int(RSAServer[516])));

		cout << "Connected on port " << (int(RSAServer[515] << 8)|int(RSAServer[516])) << endl;
	}
	else
	{
		cout << "Inauthentic Server" << endl;
		return(0);
	}

	return(0);
}

bool Auth_Server(cpp_int RSA_N, cpp_int RSA_E)
{
	ifstream CertFile("./Certificate");
	char Cert[3500];
	cpp_int Cert_N, Cert_E;
	if(!CertFile.is_open())
	{
		cout << "Certificate file not available." << endl;
		return(false);
	}

	CertFile.getline(Cert, 3500, char(5));
	for(int i = 0; i < 9; i++)
	{
		if(i == 0)
		{
			int first_num = strcspn(Cert, ":")+2;	//Find the first number, public key value e, and store it to Cert_E
			int second_num = strcspn(Cert, ",");
			Cert_E = Dehexer(Cert, first_num, second_num-first_num);

			first_num = second_num+1;	//Find the second number, public key value n, and store it to Cert_N
			second_num = strcspn(Cert, "\n");
			Cert_N = Dehexer(Cert, first_num, second_num-first_num);

			if(RSA_N != Cert_N || RSA_E != Cert_E)	//If the server and the certficate have different public keys, then the server is inauthentic
				return(false);
		}
	}
	return(true);
}

cpp_int Dehexer(char String[], int first_byte, int length)
{
	cpp_int Answer = 0;
	int i;

	for(i = first_byte; i < first_byte+length; i++)
	{
		Answer <<= 4;
		if(String[i] >= '0' && String[i] <= '9')	//Digits
			Answer += uint8_t(String[i]-'0');
		else if(String[i] >= 'a' && String[i] <= 'f')	//Lower case digits
			Answer += uint8_t(String[i]-'a')+10;
		else if(String[i] >= 'A' && String[i] <= 'F')	//Upper case digits
			Answer += uint8_t(String[i]-'A')+10;
		else						//We were told a length longer than a valid hex number
			break;
	}

	return(Answer);
}




























