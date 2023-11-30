#pragma once

#include <iostream>
#include "Client.hpp"
#include "Channel.hpp"

class ACommand
{
protected:
	Channel &channel_;
	Client &sender_;
public:
	// Constructors / Destructor
	ACommand(Channel &channel, Client &sender): channel_(channel), sender_(sender){}
	~ACommand();

	// Operator Overload

	// Functions
	virtual void exe() const = 0;
	Channel &getChannel(){return channel_;}
	Client &getSender(){return sender_;}
};

// Command_HPP