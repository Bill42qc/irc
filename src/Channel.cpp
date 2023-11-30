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

void Channel::sendToEveryone(const std::string &msg){
	std::map<int, Client>::iterator it;

    for (it = clientMap_.begin(); it != clientMap_.end(); ++it) {
        it->second.send(msg);
    }

}
