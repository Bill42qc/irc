#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>

const char *WELCOME_MSG = "Welcome into FT_IRC by bmarttin, pbergero and rofontai\n";
const int MAX_CONNECTIONS = 100;
const int BUFFER_SIZE = 2048;

Server::Server(){
}

Server::~Server(){
}

///@brief
//create the socket use for connection
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

///@brief
//bind the socket to the ports given the the server
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

///@brief
//listen to the socket for incomming input
void Server::listenSocket(){
	if (listen(socket_, MAX_CONNECTIONS) == -1) {
		close(socket_);
		throw ListenException();
		}

	std::cout << "Server listening on port " << port_ << "..." << std::endl;
}

///@brief
/*accept a new connection and create the socket for that connection.
Than the socket is added into the vector of socket*/
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

	clientfd.fd = clientSocket;
	clientfd.events = POLLIN;
	//add the socket of the new client to the vector
	pollfd_.push_back(clientfd);

	Client client(clientSocket);
	client.send(WELCOME_MSG);
	client.send("AUTHENTICATE\n");
	addClient(client);
}

///@brief
//handles the data send by a socket
///@param
//i : the index of the client that send data
void Server::handleClientInput(int i){

	Client &client = getClient(i);
	int	clientSocket = client.getClientSocket();
	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);
	ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesRead > 0) {
			buffer[bytesRead - 1] = 0;
			client.catMSG(buffer);
			if (buffer[bytesRead -2] == 13) {
				std::cout << "Received data from client: " << client.getMSG() << std::endl;
				client.resetMSG();
			}

	}
	else if (bytesRead == 0) {
		// Connection closed by the client
		std::cout << "Connection closed by client." << std::endl;
		close(clientSocket);
		removeClient(i);
		pollfd_.erase(pollfd_.begin()+i+1);
	}
	else {
		throw RecvException();
	}
}

///@brief
/*Main loop of the server
Poll an array of fd and wait for event to happen.
When event happen handle the data send or create new client*/
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
				try {
					if (pollfd_[i].revents & POLLIN) {
						if (pollfd_[i].fd == socket_) {
							receiveNewConnection();
						} else {
							handleClientInput(i - 1);
						}
					}
				}
				catch (std::exception &e){
					std::cout << e.what() << std::endl;
					removeClient(i - 1);
				}
			}
		}
	}
}

///@brief
//init the socket needed for the server
///@param
//port : the port number that will be listen to
///@param
//password : the password needed to connect to the server
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

///@brief
//Add a client to the server
///@param
//client : the client added to the server
void Server::addClient(Client &client){
	clientVector_.push_back(client);
}

///@brief
//remove a client from the server
///@param
//i : the index of the client in the vector
void Server::removeClient(int i){
	clientVector_[i].closeSocket();
	if (i >= 0 && i < static_cast<int>(clientVector_.size()))
		 clientVector_.erase(clientVector_.begin() + i);
}

///@brief
//add a channel to the map
void Server::addChannel(Channel &channel){
	channelVector_.push_back(channel);
}

///@brief
//join a channel if it doesn exist create it if its not there yet
///@param
//name : the name of the channel that will be join
void Server::joinChannel(std::string name, Client &client) {
	for (unsigned long i = 0;  i < channelVector_.size(); ++i)
	{
		if (channelVector_[i] == name){
			channelVector_[i].addClient(client);
			return ;
		}
	}

	Channel newChannel(name);
	newChannel.addClient(client);
	addChannel(newChannel);
}

Channel &Server::getChannel(std::string &name){
	for (unsigned long i = 0;  i < channelVector_.size(); ++i){
		if (channelVector_[i] == name){
			return channelVector_[i];
		}
	}
	throw (std::runtime_error("channel does not exist"));
}
