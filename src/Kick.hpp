#pragma once

#include <iostream>
#include "ACommand.hpp"

class Client;
class Channel;
class Kick: public ACommand
{
private:

public:
	// Constructors / Destructor
	Kick(Channel &channel, Client &sender, std::vector<std::string> v);
	~Kick();

	// Functions
	void exe() const;
};

// Kick_HPP