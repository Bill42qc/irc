#pragma once

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

class Client
{
private:
	std::string hostName_;
	std::string msg_;
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

	// Functions
	void	send(std::string msg);
	void closeSocket();

	// getter setter
	const std::string &getHostName() const{return hostName_;}
	const std::string &getNickName() const{return nickName_;}
	const std::string &getUserName() const{return userName_;}
	int getClientSocket() const{return clientSocket_;}
	bool getHasPassword(){return hasPassword_;}
	void setNickName(std::string &name){nickName_ = name;}
	void setUserName(const std::string &name) { userName_ = name; }
	void setHostName(const std::string &name) { hostName_ = name; }
	void validatePassword(){hasPassword_ = true;}
	void catMSG(std::string msg){msg_ = msg_ + msg;}
	void rmCarReturnMSG(){msg_ = msg_.substr(0, msg_.size() - 1);}
	void resetMSG(){msg_ = "";}
	const std::string &getMSG() const {return msg_;}
	bool operator<(const Client& other) const {return (clientSocket_ < other.clientSocket_);} //overload so it can be use for map
	bool operator!=(const Client& other) const {return (clientSocket_ != other.clientSocket_);}//overload for iterator throw map
	bool operator==(const Client& other) const {return (clientSocket_ == other.clientSocket_);}
};

// Client_HPP