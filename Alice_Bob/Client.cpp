#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio;
using ip::tcp;

int main() {
    io_service io;
    tcp::socket socket(io);
    socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 12345));

    array<char, 128> buf;
    boost::system::error_code error;
    socket.receive(buffer(buf));
    cout << "Received: " << buf.data() << endl;

    string message = "Hello, server!";
    socket.send(buffer(message));

    return 0;
}
