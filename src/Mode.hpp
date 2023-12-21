#pragma once

#include <iostream>
#include "ACommand.hpp"
#include "utility.hpp"

class Mode: public ACommand
{
private:

public:
	// Constructors / Destructor
	Mode(Channel &channel, Client &sender, std::vector<std::string> v): ACommand(channel, sender, v){};
	~Mode();
	// Functions
	bool removeMode(char modeChar, size_t *i) const;
	bool AddMode(char modeChar, size_t *i) const;
	void exe() const;
};

// Mode_HPP