#include "Server.hpp"

void Server::parsMsg(std::string const &recept, Client &client)
{
	command_ = splitString(recept, 32);

	// if (command_[0] == "CAP")
	// 	client.send(RPL_WELCOME(client.getNickName(), client.getUserName(), client.getHostName()));
	if(command_[0] == "PING"){
		handlePing(client);
	}
	if(command_[0] == "JOIN"){
		join(client);
	}
	if (command_[0] == "NICK") {
		nick(client);
	}
	if (command_[0] == "PASS") {
		pass(client);
	}
	if (command_[0] == "PRIVMSG") {
		privmsg(client);
	}
}

void Server::nick(Client &client){
	std::cout << client.getNickName() << ": je suis la " << std::endl;
	std::cout << command_[1] << " je aussi ici " << std::endl;
	client.send(RPL_NICK(client.getNickName(), command_[1]));
	client.setNickName(command_[1]);
}

void Server::pass(Client &client){
	client.setHasPassword();
	client.setPassword((command_[1]));
	password_check(serverPassword_, client.getPassword());
}

void Server::privmsg(Client &client){
	std::cout << "this shit still not done " << client.getNickName() <<std::endl;
}

void Server::join(Client &client)
{
	//verification pour rejoindre channel
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
			client.send(RPL_JOIN(client.getNickName(), command_[1]));
			client.send(RPL_TOPIC(client.getNickName(), command_[1], getChannel(command_[1]).getTopic()));
		}
		catch (std::exception &e){
			std::cerr << e.what() << CRLF;
		}
	}
	catch(std::exception){
		joinChannel(command_[1], client);
		client.setUserName("user");
		client.setHostName("host");
		std::cout << "nick: " << client.getNickName() << std::endl << "User: " << client.getUserName() << std::endl << "Host: " << client.getHostName() << std::endl << "channel: " << getChannel(command_[1]).getName() << std::endl;
		client.send(RPL_JOIN(client.getNickName(), command_[1]));
		if(getChannel(command_[1]).getTopic() != "")
			client.send(RPL_TOPIC(client.getNickName(), getChannel(command_[1]).getName(), getChannel(command_[1]).getTopic()));
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
		Channel &channel = getChannel(command_[1]);

		if (command == "TOPIC")
			return (new Topic(channel, client, command_));
		if (command == "KICK")
			return (new Kick(channel, client, command_));
		if (command == "INVITE")
			return (new Invite(channel, client, command_));
		if (command == "MODE")
			return (new Mode(channel, client, command_));
	}
	catch (std::exception &e){
			client.send("no such channel"); // envoyer la bonne erreur 
	}
	throw std::runtime_error(""); //will never go there but need it for compilation
}
