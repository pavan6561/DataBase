
#include <iostream>
#include <boost/asio.hpp>
#include<vector>
using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

void request_msg_client(tcp::socket & socket ,const string message)
{

	boost::system::error_code error;
	boost::asio::write( socket, boost::asio::buffer(message), error );
	if( !error ) {
		cout << "Client sent a message to server as :" << message << endl;
	}
	else {
		cout << "send failed: " << error.message() << endl;
	}
}


 void get_rsp_svr(tcp::socket  &socket)
{
	boost::system::error_code error;
	boost::asio::streambuf receive_buffer;
	boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
	if( error && error != boost::asio::error::eof ) {
		cout << "receive failed: " << error.message() << endl;
	}
	else {
		const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
		cout << "received msg from server as: "<< data << endl;
	}

}

int main() {
	boost::asio::io_service io_service;
	//socket creation
	tcp::socket socket_(io_service);
	//connection
	socket_.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 1234 ));
        cout<<"client is connected to server now \n"<< endl;
	// request/message from client
      std::vector<std::string> msg{"systems\n"};
      for (string str : msg)
	request_msg_client(socket_,str);
	// getting response from server
	get_rsp_svr(socket_);
	return 0;
}
 







