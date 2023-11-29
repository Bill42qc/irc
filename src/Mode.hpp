#pragma once

#include <iostream>
#include "ACommand.hpp"

class Mode: public ACommand
{
private:

public:
	// Constructors / Destructor
	Mode();
	~Mode();
	// Functions
	void exe() const;
};

// Mode_HPP