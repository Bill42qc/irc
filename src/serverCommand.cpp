#include "Server.hpp"
#include "utility.hpp"

bool isregisteredCommand(std::string name){
	if (name == "JOIN" || name == "PART" || name == "PRIVMSG" || name == "KICK" || name == "TOPIC" || name == "MODE" || name == "INVITE"){
		return true;
	}
	return false;
}

void Server::parsMsg(std::string const &recept, Client &client)
{
	command_ = splitString(recept, 32);

	if (command_[0] == "CAP LS 302") {
		client.setHasCapLs();
	}
	else if(command_[0] == "PING"){
		handlePing(client);
	}
	else if (command_[0] == "NICK") {
		nick(client);
	}
	else if (command_[0] == "PASS") {
		pass(client);
	}
	else if (command_[0] == "USER" && client.getAuthSent() == false) {
		user(client);
	}
	// std::cout << "pass = " << client.getHasPassword() << std::endl << "auhtsens = " << client.getAuthSent() << std::endl << "nick = " << client.getNickName() << std::endl;
	if(client.getHasPassword() == true && client.getNickName() != "*" && client.getHasUser() == true)
	{
		if(client.getAuthSent() == false)
		{
			client.setAuthSent(true);
			client.send(RPL_WELCOME(client.getNickName(), client.getUserName(), client.getHostName()));
						std::cout << GRE << "AUTHENTIFICATION COMPLETE" << WHT << std::endl;
		}

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
	else if (isregisteredCommand(command_[0]))
        client.send(ERR_NOTREGISTERED(client.getNickName()));
	return ;
}

bool Server::checkClientByNickName(std::string name){
	for (unsigned long i = 0; i < clientVector_.size(); i++){
		if (clientVector_[i].getNickName() == name)
			return true;
	}
	return false;
}

bool checkIllegalClientNickName(std::string name){
	if (name.find(':') == std::string::npos || name.find('#') == std::string::npos || name.find('@') == std::string::npos || name.find('&') == std::string::npos|| name.find(' ') == std::string::npos){
		return false;
	}
	return true;

}

void Server::nick(Client &client) {
	if (checkClientByNickName(command_[1]) == true){
		client.send(ERR_NICKNAMEINUSE(client.getNickName( ),command_[1]));
	}
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
		client.setHasNick();
	}
	return;
}


void Server::pass(Client &client){
	client.setPassword((command_[1]));
	if (client.getHasPassword() == true){
		client.send(ERR_ALREADYREGISTERED(client.getNickName()));
	}
	else if(password_check(serverPassword_, client.getPassword(), client) == true)
		{
			client.setHasPassword(true);
			client.setIsAuth();
			std::cout << GRE << "PASSWORD OK" << WHT << std::endl;
		}
}

void Server::user(Client &client){
	if (client.getHasUser() == true){
		client.send(ERR_ALREADYREGISTERED(client.getNickName()));
	}
	if (command_.size() > 5){
		client.send(ERR_NEEDMOREPARAMS(client.getNickName(), "USER"));
	}
	if (command_[1].empty() == true || command_[2].empty() == true || command_[3].empty() == true || command_[4].empty() == true ){
		client.send(ERR_NEEDMOREPARAMS(client.getNickName(), "USER"));
	}
	if (command_[1].find(' ') != std::string::npos || command_[1].find(':') != std::string::npos || command_[1].find('!') != std::string::npos || command_[1].find('*') != std::string::npos|| command_[1].find('&') != std::string::npos || command_[1].find('#') != std::string::npos || command_[1].find('@') != std::string::npos){
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

void Server::join(Client &client)
{
	if(!(command_[1][0] == '#' || command_[1][0] == '&'))
	{
		client.send(ERR_BADCHANMASK(command_[1]));
		return ;
	}
	try{
		Channel &channel = getChannel(command_[1]);
		try{
			try{
				channel.getClientByNickName(client.getNickName());
				client.send(ERR_USERONCHANNEL(client.getNickName(), client.getNickName(), channel.getName()));
			}
			catch (std::exception){ return ;}
			if(channel.getIsInviteOnly() == true){
				if(channel.isOnInviteList(client)){
					if (command_.size() > 2){
						channel.joinChannel(client, command_[2]);
						client.send(RPL_JOIN(client.getNickName(), command_[1]));
						client.send(RPL_TOPIC(client.getNickName(), command_[1], getChannel(command_[1]).getTopic()));
					}
					else{
						channel.joinChannel(client);
						client.send(RPL_JOIN(client.getNickName(), command_[1]));
						client.send(RPL_TOPIC(client.getNickName(), command_[1], getChannel(command_[1]).getTopic()));
					}
				}
				else{
					throw std::runtime_error(ERR_INVITEONLYCHAN(client.getUserName(), channel.getName()));
				}
			}
			else if (command_.size() > 2){
				channel.joinChannel(client, command_[2]);
				client.send(RPL_JOIN(client.getNickName(), command_[1]));
				client.send(RPL_TOPIC(client.getNickName(), command_[1], getChannel(command_[1]).getTopic()));
				getChannel(command_[1]).broadcastUserList(client);
			}
			else {
				channel.joinChannel(client);
				client.send(RPL_JOIN(client.getNickName(), command_[1]));
				client.send(RPL_TOPIC(client.getNickName(), command_[1], getChannel(command_[1]).getTopic()));
				getChannel(command_[1]).broadcastUserList(client);
			}
		}
		catch (std::exception &e){
			client.send(e.what());
		}
	}
	catch(std::exception) {
		joinChannel(command_[1], client);
		client.send(RPL_JOIN(client.getNickName(), command_[1]));
		client.send(RPL_TOPIC(client.getNickName(), getChannel(command_[1]).getName(), getChannel(command_[1]).getTopic()));
		getChannel(command_[1]).sendUserList(client);
		getChannel(command_[1]).setNeedPassword_(false);
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
			chan.broadcastEveryone(RPL_PART(client.getNickName(), chan.getName()));
			chan.removeClient(client);
		}
		catch (std::exception){
			client.send(ERR_NOSUCHCHANNEL(client.getNickName(), command_[1]));
		}
	}
}
