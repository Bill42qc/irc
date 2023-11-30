#pragma once

#include <iostream>
#include <sys/socket.h>

class Client
{
private:
	std::string hostName_;
	std::string nickName_;
	std::string userName_;
	int clientSocket_;
	bool hasPassword_;
public:
	// Constructors / Destructor
	Client();
	Client(int clientSocket);
	Client(const Client &other);
	~Client();

	// Operator Overload
	Client& operator=(const Client &other);
	void	send(std::string msg);
	// Functions


	// getter setter
	const std::string &getHostName() const{return hostName_;}
	const std::string &getNickName() const{return nickName_;}
	const std::string &getUserName() const{return userName_;}
	int getClientSocket() const{return clientSocket_;}
	bool getHasPassword(){return hasPassword_;}
	void setHostName(std::string &name){hostName_ = name;}
	void setNickName(std::string &name){nickName_ = name;}
	void setUserName(std::string &name){userName_ = name;}
	void validatePassword(){hasPassword_ = true;}
	bool operator<(const Client& other) const {return (clientSocket_ < other.clientSocket_);} //overload so it can be use for map
	bool operator!=(const Client& other) const {return (clientSocket_ != other.clientSocket_);}//overload for iterator throw map

};

// Client_HPP