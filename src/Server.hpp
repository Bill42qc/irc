#pragma once

#include <iostream>
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
	mutable std::vector<Client> clientVector_;
	mutable std::vector<struct pollfd> pollfd_;
	mutable std::vector<Channel> channelVector_;
	std::string	password_;
	int socket_;
	uint32_t port_;
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
	Channel &getChannel(std::string &name);
	void joinChannel(std::string name, Client &client);
	void init(const std::string &port, const std::string &password);
	void addClient(Client &client);
	void removeClient(int i);
	void addChannel(Channel &channel);
	void handleNewConnection();
	void handleData(int clientSocket);
	void run();


	Client &getClient(int i){return clientVector_[i];}

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