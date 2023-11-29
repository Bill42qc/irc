#pragma once

#include <iostream>
#include "ACommand.hpp"
class Invite: public ACommand
{
private:

public:
	// Constructors / Destructor
	Invite();

	~Invite();
	// Functions
	void exe() const;
};

// Invite_HPP