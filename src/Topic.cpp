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
		std::cout << "i'm op" << std::endl;
		if (args_.size() == 3)
		{
			channel_.setTopic("");
			channel_.broadcastEveryone(sender_.getMSG() + CRLF);
			std::cout << "i test" << std::endl;
		}
		else {
			std::cout << "i test 2" << std::endl;
			channel_.setTopic(args_[2]);
			channel_.broadcastEveryone(sender_.getMSG() + CRLF);
		}
	}
	else {
		sender_.send("not on channel");
	}
}