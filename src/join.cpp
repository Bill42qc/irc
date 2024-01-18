#include "Server.hpp"
#include "utility.hpp"


void Server::joinNotExistingChannel(Client &client, std::string channelName){
	joinChannel(channelName, client);
	client.send(RPL_JOIN(client.getNickName(), channelName));
	client.send(RPL_TOPIC(client.getNickName(), getChannel(channelName).getName(), getChannel(channelName).getTopic()));
	getChannel(channelName).sendUserList(client);
	getChannel(channelName).setNeedPassword_(false);
}

void Server::joinWithoutPassword(Client &client, Channel &channel){
	channel.joinChannel(client);
	client.send(RPL_JOIN(client.getNickName(), channel.getName()));
	client.send(RPL_TOPIC(client.getNickName(), channel.getName(), channel.getTopic()));
	channel.broadcastUserList(client);
}

void Server::joinPassword(Client &client, std::string password, Channel &channel)
{
	channel.joinChannel(client, password);
	client.send(RPL_JOIN(client.getNickName(), channel.getName()));
	client.send(RPL_TOPIC(client.getNickName(), channel.getName(), channel.getTopic()));
	channel.broadcastUserList(client);

}

void Server::join(Client &client)
{
	std::vector<std::string> channelName = splitString(command_[1], ',');
	std::vector<std::string> password; 
	if (command_.size() > 2){
		password = splitString(command_[2], ',');
	}

	for (size_t i = 0; i < channelName.size(); i++){
		if(!(channelName[i][0] == '#' || channelName[i][0] == '&'))
		{
			client.send(ERR_BADCHANMASK(channelName[i]));
			continue;
		}
		try{
			Channel &channel = getChannel(channelName[i]);
			try{
				try{
					channel.getClientByNickName(client.getNickName());
					client.send(ERR_USERONCHANNEL(client.getNickName(), client.getNickName(), channel.getName()));
					return ;
				} catch (std::exception){}
			if(channel.getIsInviteOnly() == true){
				if(!channel.isOnInviteList(client)){
					throw std::runtime_error(ERR_INVITEONLYCHAN(client.getUserName(), channel.getName()));
				}
			}
			if (password.size() <= i){
				joinWithoutPassword(client, channel);
			}else {
				joinPassword(client, password[i], channel);
			}
		}
		catch (std::exception &e){
			client.send(e.what());
		}
		}
		catch(std::exception) {
			joinNotExistingChannel(client, channelName[i]);
		}
	}
}