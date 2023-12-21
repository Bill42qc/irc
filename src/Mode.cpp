#include "Mode.hpp"

Mode::~Mode(){
}

void Mode::exe() const
{

	size_t lastArgUse = 3;
	bool addOrSub = true;
	bool unknownMode = false;
	std::string unknownModeString = "";
	if (args_.size() == 2){
		sender_.send(RPL_CHANNELMODEIS(sender_.getNickName(), channel_.getName(), channel_.getMode()));
		return;
	}
	if (channel_.isOperator(sender_) == false){
		sender_.send(ERR_CHANOPRIVSNEEDED(sender_.getNickName(), channel_.getName()));
		return;
	}
	for (unsigned long i = 0; i < args_[2].size(); i++){
		if (args_[2][i] == '-'){
			addOrSub = false;
		}
		else if (args_[2][i] == '+'){
			addOrSub = true;
		}
		else if (addOrSub == true){
			if (AddMode(args_[2][i], &lastArgUse) == false)
			{
				unknownMode = true;
				unknownModeString += args_[2][i];
			}
		}
		else {
			if (removeMode(args_[2][i]) == false)
			{
				unknownMode = true;
				unknownModeString += args_[2][i];
			}
		}
		if (unknownMode == true){
			sender_.send("placeholder");
		}
	}
	

}

bool Mode::removeMode(char modeChar) const{
	if (modeChar == 'i'){
		channel_.setInviteOnly(false);
		return true;
	}
	if (modeChar == 't'){
		channel_.setIsTopicLimited_(false);
		return true;
	}
	if (modeChar == 'o'){
		try
		{
			if (args_.size() < 4){
				Client &deletedOP = channel_.getClientByNickName(args_[3]);
				channel_.removeOperator(deletedOP);
				channel_.broadcastUserList(sender_);
			}
			
		}
		catch(const std::exception& e)
		{
		}
		
		return true;
	}
	if (modeChar == 'l'){
		channel_.setIsClientLimited_(false);
		return true;
	}
	if (modeChar == 'k'){
		channel_.setNeedPassword_(false);
		return true;
	}
	return false;
}

bool Mode::AddMode(char modeChar, size_t *i) const{

	if (modeChar == 'i'){
		channel_.setInviteOnly(false);
		return true;
	}
	if (modeChar == 't'){
		channel_.setIsTopicLimited_(false);
		return true;
	}
	if (modeChar == 'o'){
		if (args_.size() > *i)
		{
			try{
				Client &newOperator = channel_.getClientByNickName(args_[*i]);
				channel_.addOperator(newOperator);
				channel_.broadcastUserList(sender_);
			}
			catch (std::exception &e){
				sender_.send(ERR_USERNOTINCHANNEL(sender_.getNickName(), args_[*i], channel_.getName()));
			}
			*i = *i + 1;
		}
		return true;
	}
	if (modeChar == 'l'){
		if (args_.size() > *i)
		{
			try{
				unsigned int limit = std::stoi(args_[*i]);
				channel_.setUserLimit(limit);
				channel_.setIsClientLimited_(true);
			}
			catch (std::exception &e){}
			*i = *i + 1;
		}
		return true;
	}
	if (modeChar == 'k'){
		if (args_.size() > *i)
		{
			channel_.setNeedPassword_(true);
			channel_.setPassword(args_[*i]);
			*i = *i + 1;
		}
		return true;
	}
	return false;
}