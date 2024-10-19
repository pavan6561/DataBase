#include <iomanip>
#include <iostream>
#include <sstream>

#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID 0x1111
#define SAMPLE_INSTANCE_ID 0x2222
#define ADD_METHOD_ID 0x01

std::shared_ptr<vsomeip::application> app;

void add(const std::shared_ptr<vsomeip::message> &_request ){
std::cout<<__func__<<std::endl;
int a,b;
std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();

          a= (int)*(its_payload->get_data()+0);

          b= (int)*(its_payload->get_data()+1);

           std::cout<<"sum :"<<a+b<<std::endl;

}


int main() {

   app = vsomeip::runtime::get()->create_application("server");
   app->init();
   app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, ADD_METHOD_ID, add);
   app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
   app->start();
}