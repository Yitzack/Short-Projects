#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdint>
#include<utility>
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
cpp_int Dehexer(uint8_t[], int, int);	//Changes a number stored as hexidecimal as characters into integer
tuple<uint8_t**,int*,int> Split(const char[], const char);	//Split a character array into an array of uint8_t arrays and the number of splits based on a given character

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
	tuple<uint8_t**,int*,int> Lines;
	cpp_int Cert_N, Cert_E;
	int first_num, second_num;

	if(!CertFile.is_open())
	{
		cout << "Certificate file not available." << endl;
		return(false);
	}

	CertFile.getline(Cert, 3500, char(5));
	Lines = Split(Cert, '\n');

	first_num = find(Lines.first[0], Lines.first[0]+strlen(Lines.first[0]), ':')+2;
	second_num = find(Lines.first[0], Lines.first[0]+strlen(Lines.first[0]), ',');
	Cert_E = Dehexer(Lines.first[0], first_num, second_num-first_num);

	first_num = second_num+1;
	second_num = strlen(Lines.first[0]);//find(Lines.first[0], Lines.first[0]+strlen(Lines.first[0]), '\0');
	Cert_N = Dehexer(Lines.first[0], first_num, second_num-first_num);

	if(RSA_N != Cert_N || RSA_E != Cert_E)
		return(false);

	for(int i = 0; i < Lines.second; i++)
		delete Lines.first[i];
	delete Lines.first;

	return(true);
}

tuple<uint8_t**,int*,int> Split(const char String[], const char Character)
{
	int i, Char_Count;
	int* locations, String_len;
	uint8_t** String_set;

	if(String == NULL || strlen(String) == 0)
		return(pair<char**,int>(NULL,0));

	Char_Count = count(String, String+strlen(String), Character);

	locations = new int[Char_Count+1];
	String_len = new int[Char_Count];
	String_set = new char*[Char_Count];
	locations[0] = 0;

	i = 1;
	for(int j = 0; j < strlen(String); j++)
	{
		if(String[j] == Character)
		{
			locations[i] = j;
			i++;
		}
	}

	for(i = 0; i < Char_Count; i++)
	{
		String_set[i] = new char[locations[i+1]-locations[i]];
		String_len[i] = locations[i+1]-locations[i];
		memcpy(String_set[i], &String[locations[i]], locations[i+1]-locations[i]);
		String_set[i][locations[i+1]-locations[i]] = 0;
	}

	return(tuple<uint8_t**,int*,int>(String_set,String_len,Char_Count));
}

cpp_int Dehexer(uint8_t String[], int first_byte, int length)
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




























