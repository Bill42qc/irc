#include "Invite.hpp"


Invite::~Invite(){
}


void Invite::exe() const
{
	try {
		if (args_.size() != 3) {
			sender_.send(ERR_NEEDMOREPARAMS(sender_.getNickName(), args_[0]));
			return ;
		}
		Client &invit = serv_.getClientByNickName(args_[1]);
		try {
			channel_.getClientByNickName(invit.getNickName());
			sender_.send(ERR_USERONCHANNEL(sender_.getNickName(), args_[1], channel_.getName()));
		}
		catch (std::exception){
			if (channel_.getIsInviteOnly() == true) {
				if (channel_.isOperator(sender_) == false)
					sender_.send(ERR_CHANOPRIVSNEEDED(sender_.getNickName(), channel_.getName()));
				else {
					channel_.addInviteList(invit);
					sender_.send(RPL_INVITING(sender_.getNickName(), args_[1], channel_.getName()));
					invit.send(RPL_INVITE(sender_.getNickName(), invit.getNickName(), channel_.getName()));
				}
			}
			else {
				sender_.send(RPL_INVITING(sender_.getNickName(), args_[1], channel_.getName()));
				invit.send(RPL_INVITE(sender_.getNickName(), invit.getNickName(), channel_.getName()));
			}
		}
	}
	catch(std::exception &e) {
		return ;
	}
}
