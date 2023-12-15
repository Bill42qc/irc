#include "Kick.hpp"
const std::string DEFAULT_KICK_MSG = "because he didn't like you. sad face :(";

Kick::Kick(Channel &channel, Client &sender, std::vector<std::string> v): ACommand(channel, sender, v)
{

}
Kick::~Kick(){
}

void Kick::exe() const
{
	if (args_.size() == 1){
		sender_.send(ERR_NEEDMOREPARAMS(sender_.getNickName(), "KICK"));
		return ;
	}
	if (channel_.isOperator(sender_)){
		try{
			Client &kicked = channel_.getClientByNickName(args_[2]);
			size_t i = sender_.getMSG().find(':');
			if (i == std::string::npos) {
				kicked.send(RPL_KICKED(sender_.getNickName(), channel_.getName(), kicked.getNickName(), DEFAULT_KICK_MSG));
				sender_.send(RPL_KICKER(channel_.getName(), kicked.getNickName(), DEFAULT_KICK_MSG));
				channel_.removeClient(kicked);
			}
			else{
				std::string kickMSG = sender_.getMSG().substr(i + 1);
				kicked.send(RPL_KICKED(sender_.getNickName(), channel_.getName(), kicked.getNickName(), kickMSG));
				sender_.send(RPL_KICKER(channel_.getName(), kicked.getNickName(), kickMSG));
				channel_.removeClient(kicked);
			}
		}
		catch (std::exception &e){
			sender_.send(ERR_USERNOTINCHANNEL(sender_.getNickName(), args_[2], channel_.getName())); // add ERR 441
		}
	}
	else{
		sender_.send(ERR_CHANOPRIVSNEEDED(sender_.getNickName(), channel_.getName())); // add ERR 442
	}
}