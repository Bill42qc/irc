#include "Kick.hpp"

Kick::Kick(Channel &channel, Client &sender): ACommand(channel, sender), kick_(sender)
{
	// kick_ = sender;
}
Kick::~Kick(){
}

void Kick::exe() const
{
	if (channel_.isOperator(sender_))
	{
		sender_.send("you kicking " + kick_.getHostName());
		channel_.removeClient(kick_);
		channel_.broadcastEveryone(kick_.getNickName() + "a été ban");
	}
}