#pragma once

#include <iostream>
#include "ACommand.hpp"

class Client;
class Channel;
class Kick: public ACommand
{
private:
	Client &kick_;
public:
	// Constructors / Destructor
	Kick(Channel &channel, Client &sender);
	~Kick();

	// Functions
	void exe() const;
};

// Kick_HPP