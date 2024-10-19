#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include<string>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;
string read_(tcp::socket & socket) {
       boost::asio::streambuf buf;
       boost::asio::read_until( socket, buf, "\n" );
       string data = boost::asio::buffer_cast<const char*>(buf.data());
       return data;
}
void send_(tcp::socket & socket, const string& message) {
       const string msg = message + "\n";
       boost::asio::write( socket, boost::asio::buffer(message) );
       cout << "Server sent a message to Client as :" << message << endl;
}

int main() {
      boost::asio::io_service io_service;
//listen for new connection
      tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234 ));
      cout<<"server is running now ...."<<endl;
//socket creation 
      tcp::socket socket_(io_service);
//waiting for connection
      cout<<"server waitting for client ..."<<endl;
      acceptor_.accept(socket_);
//read operation
      string message = read_(socket_);
      cout << "recevied msg from client as : " << message << endl;
//write operation
      //  std::vector<string> msg{"Avin"};
      //  for(string str:msg)
       const string str = "Avin";
      send_(socket_,str);
   return 0;
}
































