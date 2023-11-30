#pragma once

#include <map>
#include "Client.hpp"
class Channel
{
private:
	std::string name_;
	std::string topic_;
	std::string password_;
	std::map<int, Client> clientMap_;
	std::map<int, Client> operatorMap_;
	Channel();
public:
	// Constructors / Destructor
	Channel(const std::string &name);
	~Channel();

	// Functions
	void setPassword(std::string password){password_ = password;}
	void setTopic(std::string topic){topic_ = topic;}
	bool enterPassword(std::string password){return (password == password_ ? true : false);};
	void addClient(Client &client);
	void removeClient(const Client &client);
	void addOperator(Client &client);
	void removeOperator(Client &client);
	bool isOperator(Client &client);
	void sendToEveryone(const std::string &msg);
	std::string &getName() {return name_;}
	bool operator<(const Channel& other) const {return (name_ < other.name_);}//overload so it can be use for map
};

// Channel_HPP