#pragma once

#include <iostream>
#include "ACommand.hpp"

class Kick: public ACommand
{
private:

public:
	// Constructors / Destructor
	Kick();
	~Kick();

	// Functions
	void exe() const;
};

// Kick_HPP