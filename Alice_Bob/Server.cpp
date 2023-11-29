#include<iostream>
#include<fstream>
#include<cstring>
#include<list>
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

class Client
{
	public:
		Client(int);
		~Client(){};
		Client(const Client& other);			// Copy constructor
		Client(Client&& other) noexcept;		// Move constructor
		Client& operator=(const Client& other);	// Copy assignment operator
		Client& operator=(Client&& other) noexcept;	// Move assignment operator
		void Send_Message(uint8_t[], int) const;
		void Recieve_Message(uint8_t[], int&) const;
	private:
		RSA RSA_Encryption;
		AES AES_Encryption;
		io_service io;
		tcp::acceptor acceptor;
		int socket_number;
};

Client::Client(int socket_num):io(),acceptor(io, tcp::endpoint(tcp::v4(), socket_num))
{
	socket_number = socket_num;
}

// Copy constructor
Client::Client(const Client& other)
	: RSA_Encryption(other.RSA_Encryption), AES_Encryption(other.AES_Encryption),
	  io(), acceptor(io, tcp::endpoint(tcp::v4(), socket_number)), socket_number(other.socket_number)
{}

// Move constructor
Client::Client(Client&& other) noexcept
	: RSA_Encryption(move(other.RSA_Encryption)), 
	  AES_Encryption(move(other.AES_Encryption)),
	  io(), acceptor(move(other.acceptor)),
	  socket_number(other.socket_number)
{}

// Copy assignment operator
Client& Client::operator=(const Client& other)
{
	if (this != &other)
	{
		RSA_Encryption = other.RSA_Encryption;
		AES_Encryption = other.AES_Encryption;
		// Copy or handle other members
		acceptor.close(); // Close the previous acceptor
		acceptor.open(tcp::v4());
		acceptor.bind(tcp::endpoint(tcp::v4(), socket_number));
		acceptor.listen();
	}
	return *this;
}

// Move assignment operator
Client& Client::operator=(Client&& other) noexcept
{
	if (this != &other) {
		RSA_Encryption = move(other.RSA_Encryption);
		AES_Encryption = move(other.AES_Encryption);
		socket_number = move(other.socket_number);
		acceptor.close(); // Close the previous acceptor
		acceptor = move(other.acceptor); // Move-assign the acceptor
	}
	return *this;
}

void Client::Send_Message(uint8_t Message[], int Length) const
{
}

void Client::Recieve_Message(uint8_t Message[], int& Length) const
{
}

int main()
{
	Initalize_RSA();
	io_service io;	//I/O Context
	tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 50624));	//Create a connection acceptor into the I/O context
	int last_used_port = 50624;
	list<Client> Client_List;

	//while(true)
	{
		tcp::socket socket(io);	//Creat a socket in the I/O Context
		cout << "Server is ready for clients." << endl;
		acceptor.accept(socket);	//Accept a connection on the socket, passively wait for a connection

		uint8_t message[1024];		//Before the client can send a message, send it a hello, it is looking for the hello
		Hamming Hmessage[9];
		boost::system::error_code ignored_error;	//Error Code that should be used in sending and receiving data. The error code can't be used unless a flag is also set.
		cpp_int Number = RSA_Encryption.Public_key_n();	//Construct a message of the server's public keys and the port to communicate over
		for(int i = 503; i >= 0; i--)
		{
			message[i] = uint8_t(Number & 0xFF);
			Number >>= 8;
		}
		message[504] = '\n';
		Number = RSA_Encryption.Public_key_e();
		message[505] = uint8_t((Number & 0xFF0000) >> 16);
		message[506] = uint8_t((Number & 0xFF00) >> 8);
		message[507] = uint8_t(Number & 0xFF);
		message[508] = '\n';
		last_used_port++;
		Number = last_used_port;
		message[509] = uint8_t((Number & 0xFF00) >> 8);
		message[510] = uint8_t(Number & 0xFF);
		for(int i = 511; i < 1024; i++)
			message[i] = 0;
		Client_List.push_back(Client(last_used_port));	//Place the new client in the client list

		for(int i = 0; i < 9; i++)	//Convert the message from uint8_t[] to Hamming[] and store in uint8_t[]
			Hmessage[i].Encode(&message[i*62]);
		for(int i = 0; i < 9; i++)
		{
			Number = Hmessage[i].Export_Encoding();
			for(int j = 63; j >= 0; j--)
			{
				message[i*64+j] = uint8_t(Number & 0xff);
				Number >>= 8;
			}
		}
		socket.send(buffer(message,576));	//Send the message

		uint8_t buf[1024];	//I don't know why a chacter array (either the primary datatype or std) has to be used to receive but a string can used to send.
		socket.receive(buffer(buf,1024));	//Receive the reply
		cout << "Server Received: " << hex << endl;
		for(int i = 0; i < 576; i++)
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
	oRSA_Keys << Cert << "Signature: " << RSA_Encryption.Sign(Hash, 8) << endl;
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






