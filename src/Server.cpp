#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>

const char *WELCOME_MSG = "Welcome into FT_IRC by bmarttin, pbergero and rofontai\n";
const int MAX_CONNECTIONS = 100;
const int BUFFER_SIZE = 1024;

Server::Server(){
}

Server::~Server(){
}

void Server::createSocket()
{
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == -1) {
		std::cerr << "Error creating socket." << std::endl;
	   throw SocketException();
	int i = 1;
	if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)))
		throw SocketException();

	if (fcntl(socket_, F_SETFL, O_NONBLOCK) == -1) 
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

	std::cout << "Server listening on port " << port_ << "..." << std::endl;
}

void Server::receiveNewConnection(){
	int clientSocket;
	sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);
	struct pollfd clientfd;

	// Accept a connection from a client
	clientSocket = accept(socket_, (struct sockaddr*)&clientAddress, &clientAddressSize);
	if (clientSocket == -1) {
		std::cerr << "Error accepting connection." << std::endl;
		throw AcceptException();
	}

	//add the socket of the new client to the vector
	clientfd.fd = clientSocket;
	clientfd.events = POLLIN;
	pollfd_.push_back(clientfd);

	Client client(clientSocket);
	client.send(WELCOME_MSG);
	addClient(client);
}

void Server::handleClientInput(int i){

	Client client= getClient(i);
	int	clientSocket = client.getClientSocket();
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesRead > 0) {
		// Process the received data
		//this is where the fun begins 
		std::cout << "Received data from client: " << buffer << std::endl;
		client.send("received data : "+ std::string(buffer));
	} 
	else if (bytesRead == 0) {
		// Connection closed by the client
		std::cout << "Connection closed by client." << std::endl;
		close(clientSocket);
		removeClient(i);
	} 
	else {
		throw RecvException();
	}
}

void Server::run(){
	struct pollfd serverfd;
	
	serverfd.fd = socket_;
	serverfd.events = POLLIN;
	pollfd_.push_back(serverfd);

	while(true){
		int	result =  poll(pollfd_.data(), pollfd_.size(), -1);
		if (result == -1)
			throw PollException();
		else if (result > 0) {
			for (size_t i = 0; i < pollfd_.size(); ++i) {
				if (pollfd_[i].revents & POLLIN) {
					if (pollfd_[i].fd == socket_) {
						receiveNewConnection();
					} else {
						handleClientInput(i - 1);
					}
				}
			}
		}
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

void Server::addClient(Client client){
	clientVector.push_back(client);
}

void Server::removeClient(int i){
	if (i >= 0 && i < static_cast<int>(clientVector.size())) 
		 clientVector.erase(clientVector.begin() + i);
}

void Server::createChannel(const std::string &name){
	channelMap_.insert(std::make_pair(name, Channel(name)));
}

