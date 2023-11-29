#include "Server.hpp"

Server::Server(){
}

Server::~Server(){
}

void Server::init(const std::string &port, const std::string &password){
	try{
		port_ = std::stoi(port);
		password_ = password;

		//do your shit here billy pour qu'on puissent ce connecter au serveur

		for(;;){

		}
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}
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

