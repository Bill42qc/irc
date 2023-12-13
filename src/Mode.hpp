#pragma once

#include <iostream>
#include "ACommand.hpp"

class Mode: public ACommand
{
private:

public:
	// Constructors / Destructor
	Mode(Channel &channel, Client &sender, std::vector<std::string> v): ACommand(channel, sender, v){};
	~Mode();
	// Functions
	void exe() const;
};

// Mode_HPP