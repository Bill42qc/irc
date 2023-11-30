#include "Client.hpp"

Client::Client(){
}

Client::Client(std::string &nickName, std::string &userName, std::string &hostName, int clientSocket):hostName_(hostName), nickName_(nickName), userName_(userName), clientSocket_(clientSocket){	
}

Client::Client(int clientSocket):clientSocket_(clientSocket){	
}

Client::Client(const Client &other){
	*this = other;
}

Client::~Client(){
}

void Client::send(std::string msg){
	::send(clientSocket_, msg.c_str(), msg.size(), 0);
}

Client& Client::operator=(const Client &other){
	if (this != &other) {
		userName_ = other.userName_;
		hostName_ = other.hostName_;
		nickName_ = other.nickName_;
		clientSocket_ = other.clientSocket_;
	}
	return *this;
}