#include "Topic.hpp"

Topic::~Topic(){
}

void Topic::exe() const
{
	channel_.setTopic("j'aime faires des tests");
	channel_.broadcastEveryone("topix is: " + channel_.getTopic());
}