#include "Channel.hpp"

Channel::Channel(const std::string &name): name_(name){
}

Channel::Channel(): name_(){
}

Channel::~Channel(){
}

void Channel::addClient(Client &client){
	clientMap_.insert(std::make_pair(client.getClientSocket(), client));
}

void Channel::removeClient(const Client &client){
	clientMap_.erase(client.getClientSocket());
}

void Channel::addOperator(Client &client){
	operatorMap_.insert(std::make_pair(client.getClientSocket(), client));
}

void Channel::removeOperator(Client &client){
	operatorMap_.erase(client.getClientSocket());
}

bool Channel::isOperator(Client &client){
    std::map<int, Client>::const_iterator it;
    
    for (it = operatorMap_.begin(); it != operatorMap_.end(); ++it) {
        if (it->second.getClientSocket() == client.getClientSocket()) {
            return true; 
        }
    }
    return false;
}

void Channel::sendToEveryone(const std::string &msg){
	std::map<int, Client>::iterator it;

    for (it = clientMap_.begin(); it != clientMap_.end(); ++it) {
        it->second.send(msg);
    }

}
