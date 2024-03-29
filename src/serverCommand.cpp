#include "Server.hpp"
#include "utility.hpp"

void Server::parsMsg(std::string const &recept, Client &client)
{
	command_ = splitString(recept, 32);

	if (command_[0] == "CAP LS 302") {
		client.setHasCapLs();
	}
	if(command_[0] == "PING"){
		handlePing(client);
	}
	if (command_[0] == "NICK") {
		nick(client);
	}
	if (command_[0] == "PASS") {
		pass(client);
	}
	if (command_[0] == "USER") {
		user(client);
	}
	if (client.getAuthSent() == false)
		authenticate(client);
	if(client.getAuthSent() == true)
	{
		if (command_[0] == "PRIVMSG") {
			privmsg(client);
			return;
		}
		if(command_[0] == "JOIN"){
			join(client);
			return;
		}
		if (command_[0] == "PART") {
			part(client);
			return;
		}
	}
	return ;
}

void Server::authenticate(Client &client){
	if (client.getAuthSent() == true)
		return ;
	if (client.getHasUser() == true && client.getHasNick() == true && client.getHasPassword() == true){
		client.setAuthSent(true);
		client.send(RPL_WELCOME(client.getNickName(), client.getUserName(), client.getHostName()));
		std::cout << GRE << "AUTHENTIFICATION COMPLETE" << WHT << std::endl;
	}
}

bool Server::checkClientByNickName(std::string name){
	for (unsigned long i = 0; i < clientVector_.size(); i++){
		if (clientVector_[i].getNickName() == name)
			return true;
	}
	return false;
}


bool checkIllegalClientNickName(std::string name){
	if (name.find(':') == std::string::npos && name.find('#') == std::string::npos && name.find('@') == std::string::npos && name.find('&') == std::string::npos && name.find(' ') == std::string::npos && name.find('!') == std::string::npos && name.find('?') == std::string::npos && name.find('$') == std::string::npos){
		return false;
	}
	return true;
}

void Server::nick(Client &client) {
	if (checkClientByNickName(command_[1]) == true)
		client.send(ERR_NICKNAMEINUSE(client.getNickName( ),command_[1]));
	else if (checkIllegalClientNickName(command_[1]) == true){
		client.send(ERR_ERRONEUSNICKNAME(client.getNickName(), command_[1]));
	}
	else if (command_[1].empty() == true){
		client.send(ERR_NONICKNAMEGIVEN(client.getNickName()));
	}
	else {
		client.send(RPL_NICK(client.getNickName(), command_[1]));
		for (size_t j = 0; j < channelVector_.size(); ++j){
			channelVector_[j].updateUser(client, command_[1]);
		}
		client.setNickName(command_[1]);
		client.setHasNick(true);
	}
	return;
}


void Server::pass(Client &client){
	client.setPassword((command_[1]));
	if(password_check(serverPassword_, client.getPassword(), client) == true)
		{
			client.setHasPassword(true);
			std::cout << GRE << "PASSWORD OK" << WHT << std::endl;
		}
}

void Server::user(Client &client){
	if (client.getHasUser() == true){
		client.send(ERR_ALREADYREGISTERED(client.getNickName()));
		return ;
	}
	if (command_.size() > 5){
		client.send(ERR_NEEDMOREPARAMS(client.getNickName(), "USER"));
		return ;
	}
	if (command_[1].empty() == true || command_[2].empty() == true || command_[3].empty() == true || command_[4].empty() == true ){
		client.send(ERR_NEEDMOREPARAMS(client.getNickName(), "USER"));
		return ;
	}
	if (command_[1].find(' ') != std::string::npos || command_[1].find(':') != std::string::npos || command_[1].find('!') != std::string::npos || command_[1].find('*') != std::string::npos|| command_[1].find('&') != std::string::npos || command_[1].find('#') != std::string::npos || command_[1].find('@') != std::string::npos || command_[2] != "0" || command_[3] != "*"){
		command_[1] = client.getNickName();
	}
	std::string username = command_[1];
	client.setUserName(username);
	client.setHasUser(true);
}

void Server::privmsg(Client &client){


	size_t i = client.getMSG().find(':');
	std::string msg;
	if (i == std::string::npos) {
		client.send(ERR_NEEDMOREPARAMS(client.getNickName(), "PRIVMSG"));
	}
	else{
		msg = client.getMSG().substr(i + 1);
	}
	if (command_[1][0] == '#'){
		try {
			Channel &chan = getChannel(command_[1]);
			try{
				chan.getClientByNickName(client.getNickName());
			}
			catch (std::exception){
				client.send(ERR_NOTONCHANNEL(client.getNickName(), chan.getName()));
				return ;
			}
			chan.broadcastEveryoneElse(RPL_MSGCHANNEL(client.getNickName(), chan.getName(), msg), client);
		}
		catch (std::exception){
			client.send(ERR_NOSUCHCHANNEL(client.getNickName(), command_[1]));
		}
	}
	else {
		try {
			Client &recipient = getClientByNickName(command_[1]);
			recipient.send(RPL_MSGONECLIENT(client.getNickName(), recipient.getNickName(), msg));
		}
		catch (std::exception){
			client.send(ERR_NOSUCHNICK(client.getNickName(), command_[1]));
		}
	}
}

///@brief
//join a channel if it doesn exist create it if its not there yet
///@param
//name : the name of the channel that will be join
void Server::joinChannel(std::string name, Client &client) {
	std::cout << "creating new channel " << name << std::endl;
	for (unsigned long i = 0;  i < channelVector_.size(); ++i)
	{
		if (channelVector_[i] == name){
			channelVector_[i].addClient(client);
			return ;
		}
	}

	Channel newChannel(name);
	addChannel(newChannel);
	getChannel(name).addClient(client);
	getChannel(name).addOperator(client);
}

ACommand *Server::commandFactory(Client &client){

	std::string command = command_[0];
	if (!(command == "TOPIC" || command == "KICK" || command == "INVITE" || command == "MODE"))
		throw std::runtime_error(""); //avoid next try catch if its not a channel command

	try{
		std::string chanName = command_[1];
		if (command == "INVITE"){
			if (command_.size() < 3){
				client.send(ERR_NEEDMOREPARAMS(client.getNickName() ,command));
			}
			chanName = command_[2];
		}
		else if (command_.size() < 2) {
				client.send(ERR_NEEDMOREPARAMS(client.getNickName() ,command));
		}
		Channel &channel = getChannel(chanName);
		try{
			channel.getClientByNickName(client.getNickName());
		}
		catch (std::exception){
			client.send(ERR_NOTONCHANNEL(client.getNickName(), channel.getName()));
			throw std::runtime_error("");
		}
		if (command == "TOPIC")
			return (new Topic(channel, client, command_));
		if (command == "KICK")
			return (new Kick(channel, client, command_));
		if (command == "INVITE")
			return (new Invite(channel, client, command_, *this));
		if (command == "MODE")
			return (new Mode(channel, client, command_));
	}
	catch (std::exception &e){
			if (command_[0] == "INVITE")
				client.send(ERR_NOSUCHCHANNEL(client.getNickName(), command_[2]));
			else
				client.send(ERR_NOSUCHCHANNEL(client.getNickName(), command_[1]));
	}
	throw std::runtime_error(""); //EVERYTHING else we just ignore it
}

void Server::part(Client &client)
{
	size_t i = client.getMSG().find(':');
	std::string msg;
	if (i == std::string::npos) {
		client.send(ERR_NEEDMOREPARAMS(client.getNickName(), "PART"));
	}
	else{
		msg = client.getMSG().substr(i + 1);
	}
	if (command_[1][0] == '#' || command_[1][0] == '&'){
		try {
			Channel &chan = getChannel(command_[1]);
			try{
				chan.getClientByNickName(client.getNickName());
			}
			catch (std::exception){
				client.send(ERR_NOTONCHANNEL(client.getNickName(), chan.getName()));
				return ;
			}
			//chan.broadcastEveryone(RPL_PART(client.getNickName(), chan.getName()));
			chan.removeClient(client);
			if (chan.getNBClient() == 0){
				removeChannel(chan.getName());
			}
		}
		catch (std::exception){
			client.send(ERR_NOSUCHCHANNEL(client.getNickName(), command_[1]));
		}
	}
}
