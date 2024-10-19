#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    
    // Create a UDP socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        exit(1);
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888); // Port number
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Server address
    
    std::string message ;
    while(1){
    std::cout << "Enter a message : ";
    std::getline(std::cin, message);
    // Send the message to the server
    sendto(clientSocket, message.c_str(), message.length(), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    
    std::cout << "Message sent to server: " << message << std::endl;

    }
    
   close(clientSocket);
    return 0;
}

