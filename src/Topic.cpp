#include "Topic.hpp"

Topic::~Topic(){
}

void Topic::setNewTopic() const{
    std::string newTopic = "";

    for (size_t i = 2; i < args_.size(); ++i){
        newTopic += args_[i] + " ";
    }
    channel_.setTopic(newTopic);
    channel_.broadcastEveryone(RPL_TOPIC(sender_.getNickName(), channel_.getName(), channel_.getTopic()));
}

void Topic::exe() const
{
    if (args_.size() == 2)
    {
        sender_.send(RPL_TOPIC(sender_.getNickName(), channel_.getName(), channel_.getTopic()));
    }
    if ((channel_.getIsTopicLimited_() == true)){

        if (channel_.isOperator(sender_)){
            setNewTopic();
        }
        else {
            sender_.send(ERR_CHANOPRIVSNEEDED(sender_.getNickName(), channel_.getName()));
        }
    }
    else {
        setNewTopic();
    }
}