#pragma once

#include <iostream>
#include <map>
#include <unistd.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "ACommand.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>

class Server
{
private:
	std::map<Client, std::string> clientMap_;
	std::map<Channel, std::string> channelMap_;
	std::string	password_;
	int socket_;
	uint32_t port_;
public:
	// Constructors / Destructor
	Server();
	~Server();

	// Functions
	void init(const std::string &port, const std::string &password);
	void addClient(Client &client);
	void removeClient(const Client &client);
	void createChannel(const std::string &name);

	class SocketException : public std::exception {
	public:
		const char* what() const throw(){return "Error creating socket.";}
	};

	class BindingException : public std::exception {
	public:
		const char* what() const throw(){return "error binding the socket";}
	};
};

// Server_HPP