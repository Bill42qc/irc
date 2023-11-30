#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>

const char *WELCOME_MSG = "Welcome into FT_IRC by bmarttin, pbergero and rofontai\n";
const int PORT = 6667;
const int MAX_CONNECTIONS = 100;
const int BUFFER_SIZE = 1024;
fd_set masterSet_;

Server::Server(){
}

Server::~Server(){
}

void handleClient(int clientSocket) {
	char buffer[BUFFER_SIZE];
	std::string welcomeMessage = "Welcome to the FT_IRC Server!\n";

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

void Server::createSocket()
{
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == -1) {
		std::cerr << "Error creating socket." << std::endl;
	   throw SocketException();
	}
}

void Server::bindSocket(){
	sockaddr_in serverAddress;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port_);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket to the specified address and port
	if (bind(socket_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		close(socket_);
		throw BindingException();
	}
}

void Server::listenSocket(){
	if (listen(socket_, MAX_CONNECTIONS) == -1) {
		close(socket_);
		throw ListenException();
		}

	std::cout << "Server listening on port " << PORT << "..." << std::endl;
}

void Server::run(){
	int clientSocket;
	sockaddr_in clientAddress;

	// Start listening for incoming connections
	while (true) {
		socklen_t clientAddressSize = sizeof(clientAddress);

		// Accept a connection from a client
		clientSocket = accept(socket_, (struct sockaddr*)&clientAddress, &clientAddressSize);
		if (clientSocket == -1) {
			std::cerr << "Error accepting connection." << std::endl;
			throw AcceptException();
		}
		Client client(clientSocket);
		client.send(WELCOME_MSG);
		// Handle the client in a separate thread or process
		handleClient(clientSocket);
	}
}

void Server::init(const std::string &port, const std::string &password){
	try{
		port_ = std::stoi(port);
		password_ = password;

		createSocket();
		bindSocket();
		listenSocket();
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

