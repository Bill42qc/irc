#include "Invite.hpp"


Invite::~Invite(){
}

void Invite::exe() const
{
	channel_.broadcastEveryone("ceci est un test pour inviter quelqu'un\n");
}