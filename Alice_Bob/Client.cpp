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
	for(i = 503; i >= 0; i--)
	{
		RSAClient[i] = uint8_t(Number & 0xFF);
		Number >>= 8;
	}
	Number = RSA_Encryption.Public_key_e();
	RSAClient[504] = uint8_t((Number & 0xFF0000) >> 16);
	RSAClient[505] = uint8_t((Number & 0xFF00) >> 8);
	RSAClient[506] = uint8_t(Number & 0xFF);
	for(i = 507; i < 576; i++)
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
	Server_Encryption.set_Public_key_e(&RSAServer[504]);
	socket.close();
	socket.connect(tcp::endpoint(tcp::v4(), int(RSAServer[507] << 8)|int(RSAServer[508])));

	cout << "Connected on port " << (int(RSAServer[507] << 8)|int(RSAServer[508])) << endl;

	return(0);
}























