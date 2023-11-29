#include "Server.hpp"

Server::Server(){
}

Server::~Server(){
}

void Server::init(){
}

void Server::addClient(Client &client){
	clientMap_.insert(std::make_pair(client, client.getHostName()));
}

void Server::removeClient(const Client &client){
	clientMap_.erase(client);
}

void Server::createChannel(const std::string &name){
	channelMap_.insert(std::make_pair(Channel(name), name));
}

