#include "Client.hpp"

Client::Client(){
}

Client::Client(std::string &nickName, std::string &userName, std::string &hostName):hostName_(hostName), nickName_(nickName), userName_(userName){	
}


Client::Client(const Client &other){
	*this = other;
}

Client::~Client(){
}

Client& Client::operator=(const Client &other){
	if (this != &other) {
		userName_ = other.userName_;
		hostName_ = other.hostName_;
		nickName_ = other.nickName_;
	}
	return *this;
}