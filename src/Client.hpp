#pragma once

#include <iostream>

class Client
{
private:
	std::string hostName_;
	std::string nickName_;
	std::string userName_;
	//int clientSocket_;
public:
	// Constructors / Destructor
	Client();
	Client(std::string &nickName, std::string &userName, std::string &hostName);
	Client(const Client &other);
	~Client();

	// Operator Overload
	Client& operator=(const Client &other);

	// Functions


	// getter setter
	std::string &getHostName(){return hostName_;}
	std::string &getNickName(){return nickName_;}
	std::string &getUserName(){return userName_;}
	void setHostName(std::string &name){hostName_ = name;}
	void setNickName(std::string &name){nickName_ = name;}
	void setUserName(std::string &name){userName_ = name;}
	bool operator<(const Client& other) const {return (hostName_ < other.hostName_);} //overload so it can be use for map
};

// Client_HPP