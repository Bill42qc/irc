#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "Server.hpp"

class Channel;
class Client;
class Server;

class Invite: public ACommand
{
private:
	Server &serv_;
public:
	// Constructors / Destructor
	Invite(Channel &channel, Client &sender, std::vector<std::string> v, Server &serv): ACommand(channel, sender, v), serv_(serv){};

	~Invite();
	// Functions
	void exe() const;
};

// Invite_HPP