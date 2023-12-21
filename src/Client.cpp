#include "Client.hpp"

Client::Client(){
}

Client::Client(int clientSocket): hostName_(""), nickName_(""), userName_(""), clientSocket_(clientSocket), hasPassword_(false), hasCapLs_(false), hasNick_(false), hasUser_(false), authSent_(false), isAuth_(false) {
}

Client::Client(const Client &other){
	*this = other;
}

Client::~Client(){
}

///@brief
//close the socket of the client
void Client::closeSocket(){
	if (clientSocket_)
		close(clientSocket_);
	clientSocket_ = 0;
}

///@brief
//send a message to the client in the channel
///@param
// msg : message that will be send to the client
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