#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio;
using ip::tcp;

int main()
{
	io_service io;	//I/O Context
	tcp::socket socket(io);	//Creat a socket in the I/O Context
	socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 50624));	//Connect the socket to the server at 127.0.0.1:50624

	char buf[1024];	//I don't know why a chacter array (either the primary datatype or std) has to be used to receive but a string can used to send.
	boost::system::error_code error;	//Error Code that should be used in sending and receiving data. The error code can't be used unless a flag is also set.
	socket.receive(buffer(buf,1024));	//Receive into the character array, a data stream from the socket and store it in the buffer.
	cout << "Received: " << buf << endl;

	string message = "Hello, server!";	//Send a message back to the server
	socket.send(buffer(message));

	return(0);
}
