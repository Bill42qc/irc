#include "Client.hpp"

Client::Client(){
}



Client::Client(int clientSocket):clientSocket_(clientSocket), hasPassword_(false){	
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
		hasPassword_ = other.hasPassword_;
	}
	return *this;
}