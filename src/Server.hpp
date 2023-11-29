#pragma once

#include <iostream>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"
#include "ACommand.hpp"

class Server
{
private:
	std::map<Client, std::string> clientMap_;
	std::map<Channel, std::string> channelMap_;
	std::string	password_;
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
};

// Server_HPP