#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include<string>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

void send_(tcp::socket & socket, std::vector<uint8_t>& message) {
	boost::asio::write( socket, boost::asio::buffer(message) );
	cout << "Server sent a image to Client "<< endl;
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

	//write operation
	cv::Mat image_o;
	string path = "/home/pavan/TCPIP_vector/l_image.png";
	image_o = cv::imread(path,1);//converting the original image to gray colour. 

	unsigned char *p=image_o.data;
	int size=image_o.total()*image_o.elemSize();
	std::vector<uint8_t> imagedata;
	for(int i=0; i<size; i++)
		imagedata.push_back(*p++);//here converting the image into bytes.
       //here sent the image data 
	send_(socket_,imagedata);
	path = '\0';
	return 0;
}
































