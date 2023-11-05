#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio;
using ip::tcp;

int main() {
    io_service io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12345));

    while (true) {
        tcp::socket socket(io);
        acceptor.accept(socket);

        string message = "Hello, client!";
        boost::system::error_code ignored_error;
        socket.send(buffer(message));

        array<char, 128> buf;
        socket.receive(buffer(buf));
        cout << "Received: " << buf.data() << endl;

        // Check the client's IP address and ignore messages from other sources if needed
        tcp::endpoint remote_endpoint = socket.remote_endpoint();
        string client_ip = remote_endpoint.address().to_string();
        cout << "Message received from IP address: " << client_ip << endl;
        // if (client_ip == "YOUR_COMPUTER_IP_ADDRESS") {
        //     // Process messages from this client
        // }
    }

    return 0;
}
