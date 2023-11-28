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
SHA256 Hashing;

int main()
{
	io_service io;	//I/O Context
	tcp::socket socket(io);	//Creat a socket in the I/O Context
	socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 50624));	//Connect the socket to the server at 127.0.0.1:50624

	uint8_t buf[1024];	//I don't know why a chacter array (either the primary datatype or std) has to be used to receive but a string can used to send.
	boost::system::error_code error;	//Error Code that should be used in sending and receiving data. The error code can't be used unless a flag is also set.
	socket.receive(buffer(buf,1024));	//Receive into the character array, a data stream from the socket and store it in the buffer.
	cout << "Client Received: " << endl << hex;
	for(int i = 0; i < 1024; i++)
	{
		if(buf[i] < 16)
			cout << 0;
		cout << uint16_t(buf[i]);
		if(i%4 == 3)
			cout << " ";
		if(i%16 == 15)
			cout << "| ";
		if(i%64 == 63)
			cout << endl;
	}
	cout << endl << dec;

	uint8_t message[] = "Hello, server!";	//Send a message back to the server
	socket.send(buffer(message));

	return(0);
}























