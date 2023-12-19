#include "Server.hpp"
#include "utility.hpp"

void Server::parsMsg(std::string const &recept, Client &client)
{
	command_ = splitString(recept, 32);

	if(command_[0] == "PING"){
		handlePing(client);
		return ;
	}
	if(command_[0] == "JOIN"){
		join(client);
		return ;
	}
	if (command_[0] == "NICK") {
		nick(client);
		return ;
	}
	if (command_[0] == "PASS") {
		pass(client);
		return ;
	}
	if (command_[0] == "PRIVMSG") {
		privmsg(client);
		return ;
	}
	if (command_[0] == "PART") {
		part(client);
	return ;
	}
}

void Server::nick(Client &client){
	client.send(RPL_NICK(client.getNickName(), command_[1]));
	client.setNickName(command_[1]);
}

void Server::pass(Client &client){
	client.setHasPassword();
	client.setPassword((command_[1]));
	password_check(serverPassword_, client.getPassword());
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
	try{
		Channel &channel = getChannel(command_[1]);
		try{
			if(channel.getIsInviteOnly() == true){
				if(channel.isOnInviteList(client)){
					if (command_.size() > 2)
						channel.joinChannel(client, command_[2]);
					else
						channel.joinChannel(client);
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
			if (command_.size() != 3){
				client.send(ERR_NEEDMOREPARAMS(client.getNickName() ,command));
			}
			chanName = command_[2];
		}
		else if (command_.size() != 2) {
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
			chan.broadcastEveryoneElse(RPL_PART(client.getNickName(), chan.getName()), client);
			chan.removeClient(client);
		}
		catch (std::exception){
			client.send(ERR_NOSUCHCHANNEL(client.getNickName(), command_[1]));
		}
	}
}
