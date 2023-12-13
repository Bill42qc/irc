#pragma once

#include <iostream>
#include "Client.hpp"
#include "Channel.hpp"

class ACommand
{
protected:
	Channel &channel_;
	Client &sender_;
	std::vector<std::string> args_;
public:
	// Constructors / Destructor
	ACommand(Channel &channel, Client &sender, std::vector<std::string> v): channel_(channel), sender_(sender), args_(v){}
	virtual ~ACommand();

	// Operator Overload

	// Functions
	virtual void exe() const = 0;
	Channel &getChannel(){return channel_;}
	Client &getSender(){return sender_;}
};

// Command_HPP