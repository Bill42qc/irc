#include "Invite.hpp"


Invite::~Invite(){
}

void Invite::exe() const
{
	if (args_.size() == 3)
	{
		if (serv_.getChannel(args_[2]).getName() != args_[2])
			sender_.send(ERR_NOSUCHCHANNEL(sender_.getNickName(), serv_.getChannel(args_[2]).getName()));

		else
			sender_.send(RPL_INVITING(sender_.getUserName(), args_[1], serv_.getChannel(args_[2]).getName()));
			sender_.send(RPL_INVITE(args_[1], serv_.getChannel(args_[2]).getName()));
	}
}

/*
	si j'ai bien 3 arguments (IVNITE, nick, channel)
		si le channel n'existe pas
			return ERR_NOSUCHCHANNEL
		sinon si le demandeur n'est pas client du channel
			return ERR_NOTONCHANNEL
		sinon si le channel est en invitation seulement et que le demandeur n'est pas operator
			return ERR_CHANOPRIVSNEEDED
		sinon si le nick est deja sur le channel
			return ERR_USERONCHANNEL
		else
			envoyer RPL_INVITING au client demandeur
			envoyer RPL_INVITE au client visé
*/