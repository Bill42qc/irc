#pragma once

#include <iostream>
class ACommand
{
private:

public:
	// Constructors / Destructor
	ACommand();
	~ACommand();

	// Operator Overload

	// Functions
	virtual void exe() const = 0;
};

// Command_HPP