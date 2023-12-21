#include "Kick.hpp"
const std::string DEFAULT_KICK_MSG = "because he didn't like you. sad face 😦 ";

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
            if (args_.size() < 4) {
                channel_.broadcastEveryone(RPL_KICKED(sender_.getNickName(), channel_.getName(), kicked.getNickName(), DEFAULT_KICK_MSG));
                channel_.removeClient(kicked);
            }
            else{
                std::string kickMSG = "";
                for (size_t i = 3; i < args_.size(); ++i){
                    kickMSG += args_[i] + " ";
                }
                channel_.broadcastEveryone(RPL_KICKED(sender_.getNickName(), channel_.getName(), kicked.getNickName(), DEFAULT_KICK_MSG));
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