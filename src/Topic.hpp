#pragma once

#include <iostream>
#include "ACommand.hpp"

class Topic: public ACommand
{
private:

public:
    // Constructors / Destructor
    Topic(Channel &channel, Client &sender, std::vector<std::string> v): ACommand(channel, sender, v){};
    ~Topic();

    // Operator Overload

    // Functions
    void exe() const;
    void setNewTopic() const;
};

// Topic_HPP