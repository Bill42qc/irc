#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>


const int PORT = 6667;
const int MAX_CONNECTIONS = 10;
const int BUFFER_SIZE = 1024;
fd_set masterSet_;

Server::Server(){
}

Server::~Server(){
}

void Server::handleNewConnection() {
    socklen_t clientAddressSize = sizeof(sockaddr_in);
    sockaddr_in clientAddress;

    // Accept a connection from a client
    int clientSocket = accept(socket_, (struct sockaddr *)&clientAddress, &clientAddressSize);
    if (clientSocket == -1) {
        std::cerr << "Error accepting connection." << std::endl;
        return;
    }

    std::cout << "Accepted connection from: " << inet_ntoa(clientAddress.sin_addr) << std::endl;

    // Add the new client socket to the master set
    FD_SET(clientSocket, &masterSet_);
	
}

void Server::handleData(int clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

    if (bytesRead <= 0) {
        std::cout << "Client disconnected." << std::endl;
        close(clientSocket);
        FD_CLR(clientSocket, &masterSet_);
    } else {
        buffer[bytesRead] = '\0'; // Null-terminate the received data
        std::cout << "Received message from client: " << buffer << std::endl;

    }
}


void Server::run() {
    // Add the server socket to the master set
    FD_SET(socket_, &masterSet_);

    while (true) {
        fd_set readSet = masterSet_;
        if (select(FD_SETSIZE, &readSet, NULL, NULL, NULL) == -1) {
            std::cerr << "Error in select." << std::endl;
            break;
        }

        // Check all sockets for activity
        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET(i, &readSet)) {
                if (i == socket_) {
                    // New connection
                    handleNewConnection();
                } else {
                    // Data available on an existing connection
                    handleData(i);
                }
            }
        }
    }
}

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    std::string welcomeMessage = "Welcome to the Simple IRC Server!\n";

    // Send a welcome message to the client
    send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);

    while (true) {
        // Receive data from the client
        int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesRead <= 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }

        buffer[bytesRead] = '\0'; // Null-terminate the received data

        // Process the received message (in this example, just echo it)
        std::cout << "Received message from client: " << buffer << std::endl;

        // Send a response back to the client
        std::string response = "Server received your message: ";
        response += buffer;
        response += "\n";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    // Close the client socket when done
    close(clientSocket);
}

void Server::init(const std::string &port, const std::string &password){
	try{
		port_ = std::stoi(port);
		password_ = password;


   int clientSocket;
    sockaddr_in serverAddress, clientAddress;

    // Create a socket
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == -1) {
        std::cerr << "Error creating socket." << std::endl;
       throw SocketException();
    }

	int reuse = 1;
	if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        
		throw SocketException();
    }

    // Set up server address struct
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    if (bind(socket_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(socket_);
        throw BindingException();
    }

    // Start listening for incoming connections
    if (listen(socket_, MAX_CONNECTIONS) == -1) {
        std::cerr << "Error listening for connections." << std::endl;
        close(socket_);
       throw ListenException();
    }

    std::cout << "Server listening on port " << PORT << "..." << std::endl;

    while (true) {
        socklen_t clientAddressSize = sizeof(clientAddress);

        // Accept a connection from a client
        clientSocket = accept(socket_, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection." << std::endl;
			throw AcceptException();
        }


        // Handle the client in a separate thread or process
        handleClient(clientSocket);
    }

    // Close the server socket (this part won't be reached in this simple example)
    close(socket_);
}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}
}

void Server::addClient(Client &client){
	clientMap_.insert(std::make_pair(client, client.getHostName()));
}

void Server::removeClient(const Client &client){
	clientMap_.erase(client);
}

void Server::createChannel(const std::string &name){
	channelMap_.insert(std::make_pair(Channel(name), name));
}

