#include "Topic.hpp"

Topic::~Topic(){
}

void Topic::exe() const
{
	if (args_.size() == 2)
	{
		sender_.send(RPL_TOPIC(sender_.getNickName(), channel_.getName(), channel_.getTopic()));
	}
	if (channel_.isOperator(sender_) == true){
		size_t i = sender_.getMSG().find(':');
		if (i != std::string::npos) {
			std::string newTopic = sender_.getMSG().substr(i + 1);
			channel_.setTopic(newTopic);
		}
		channel_.broadcastEveryone(sender_.getMSG() + CRLF);
	}
	else {
		sender_.send("not on channel");
	}
}