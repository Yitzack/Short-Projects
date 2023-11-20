#include <iostream>
#include <boost/asio.hpp>
#include"RSA.h"
#include"AES.h"
#include"SHA256.h"
#include"Hamming.h"
using namespace std;
using namespace boost::asio;
using ip::tcp;

int main()
{
	io_service io;	//I/O Context
	tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 50624));	//Create a connection acceptor into the I/O context

	//while(true)
	{
		tcp::socket socket(io);	//Creat a socket in the I/O Context
		acceptor.accept(socket);	//Accept a connection on the socket, passively wait for a connection

		string message = "Hello, client!";	//Before the client can send a message, send it a hello, it is looking for the hello
		boost::system::error_code ignored_error;	//Error Code that should be used in sending and receiving data. The error code can't be used unless a flag is also set.
		socket.send(buffer(message));	//Send the message

		char buf[1024];	//I don't know why a chacter array (either the primary datatype or std) has to be used to receive but a string can used to send.
		socket.receive(buffer(buf,1024));	//Receive the reply
		cout << "Received: " << buf << endl;

		// Check the client's IP address and ignore messages from other sources if needed
		tcp::endpoint remote_endpoint = socket.remote_endpoint();	//Record the socket remote endpoint
		string client_ip = remote_endpoint.address().to_string();	//Pull out the IP address of the remote endpoint
		cout << "Message received from IP address: " << client_ip << endl;	//Log the IP address
	}

	return(0);
}
