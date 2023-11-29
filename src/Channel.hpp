#pragma once

#include <map>
#include "Client.hpp"
class Channel
{
private:
	std::string name_;
	std::map<Client, std::string> clientMap_;
	Channel();
public:
	// Constructors / Destructor
	Channel(const std::string &name);
	~Channel();

	// Functions
	void addClient(Client &client);
	void removeClient(const Client &client);

	std::string &getName() {return name_;}
	bool operator<(const Channel& other) const {return (name_ < other.name_);}//overload so it can be use for map
};

// Channel_HPP