#include <opencv2/highgui.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include<vector>
using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

void get_rsp_svr(tcp::socket  &socket)
{
	boost::system::error_code error;
	boost::asio::streambuf receive_buffer;
	boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
	if( error && error != boost::asio::error::eof ) {
		cout << "receive failed: " << error.message() << endl;
	}
	else {
		const void* data = boost::asio::buffer_cast<const void*>(receive_buffer.data());
		void *imagedata = const_cast<void *>(data);//here converting the const void* to void*
		cv::Mat m = cv::Mat(480, 640, CV_8U, imagedata);//passing the bytes in the form of matrix.
		cv::Mat image_g=m; //here passing the matrix data into image.
		cv::namedWindow( "image", cv::WINDOW_AUTOSIZE );
		cv::imshow( "image", image_g );//printing the image in the terminal.
		cout << "received image from server " << endl;

	}

}

int main() {
	boost::asio::io_service io_service;
	//socket creation
	tcp::socket socket_(io_service);
	//connection
	socket_.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 1234 ));
	cout<<"client is connected to server now \n"<< endl;
	// getting response from server
	get_rsp_svr(socket_);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}








