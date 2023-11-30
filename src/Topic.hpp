#pragma once

#include <iostream>
#include "ACommand.hpp"

class Topic: public ACommand
{
private:

public:
	// Constructors / Destructor
	Topic(Channel &channel, Client &sender): ACommand(channel, sender){};
	~Topic();

	// Operator Overload

	// Functions
	void exe() const;

};

// Topic_HPP