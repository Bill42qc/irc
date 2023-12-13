#pragma once

#include <iostream>
#include "ACommand.hpp"

class Invite: public ACommand
{
private:

public:
	// Constructors / Destructor
	Invite(Channel &channel, Client &sender, std::vector<std::string> v): ACommand(channel, sender, v){};

	~Invite();
	// Functions
	void exe() const;
};

// Invite_HPP