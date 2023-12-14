#include "Invite.hpp"


Invite::~Invite(){
}

void Invite::exe() const
{

}

/*
	si j'ai bien 3 arguments (IVNITE, nick, channel)

		sinon si le demandeur n'est pas client du channel
			return ERR_NOTONCHANNEL
		sinon si le channel est en invitation seulement et que le demandeur n'est pas operator
			return ERR_CHANOPRIVSNEEDED
		sinon si le nick est deja sur le channel
			return ERR_USERONCHANNEL
		else
			envoyer RPL_INVITING au client demandeur
			envoyer RPL_INVITE au client visé
	else
		return ERR_ERR_NEEDMOREPARAMS
*/