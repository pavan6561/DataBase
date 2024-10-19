#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    
    // Create a UDP socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        exit(1);
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888); // Port number
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    
    // Bind the socket to the specified address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding");
        exit(1);
    }
    
    std::cout << "Server is listening on port 8888..." << std::endl;
    
    while (1) {
        socklen_t addrLen = sizeof(serverAddr);
        
        // Receive a message from the client
        int recvBytes = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &addrLen);
        buffer[recvBytes] = '\0'; // Null-terminate the received data
        
        std::cout << "Received: " << buffer << std::endl;
    }
    
   close(serverSocket);
    return 0;
}

