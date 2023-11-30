#pragma once

#include <iostream>
#include <map>
#include <unistd.h>
#include <vector>
#include <poll.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "ACommand.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <fcntl.h>

class Server
{
private:
	//variable
	std::vector<Client> clientVector;
	std::map<Channel, std::string> channelMap_;
	std::string	password_;
	int socket_;
	uint32_t port_;
	std::vector<struct pollfd> pollfd_;
	//private function
	void createSocket();
	void bindSocket();
	void listenSocket();
	void receiveNewConnection();
	void handleClientInput(int i);
public:
	// Constructors / Destructor
	Server();
	~Server();

	// Functions
	void init(const std::string &port, const std::string &password);
	void addClient(Client client);
	void removeClient(int i);
	void createChannel(const std::string &name);
	void handleNewConnection();
	void handleData(int clientSocket);
	void run();
	Client &getClient(int i){return clientVector[i];}

	class SocketException : public std::exception {
	public:
		const char* what() const throw(){return "Error creating socket.";}
	};

	class BindingException : public std::exception {
	public:
		const char* what() const throw(){return "error binding the socket";}
	};

	class ListenException : public std::exception {
	public:
		const char* what() const throw(){return "error listening the socket";}
	};

	class AcceptException : public std::exception {
	public:
		const char* what() const throw(){return "error in accepting your shitty conection";}
	};

	class PollException : public std::exception {
	public:
		const char* what() const throw(){return "error with poll";}
	};

	class RecvException : public std::exception {
	public:
		const char* what() const throw(){return "error receiving the User Input";}
	};

};

// Server_HPP