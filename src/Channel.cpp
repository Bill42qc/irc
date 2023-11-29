#include "Channel.hpp"

Channel::Channel(const std::string &name): name_(name){
}

Channel::Channel(): name_(){
}

Channel::~Channel(){
}

void Channel::addClient(Client &client){
	clientMap_.insert(std::make_pair(client, client.getHostName()));
}

void Channel::removeClient(const Client &client){
	clientMap_.erase(client);
}