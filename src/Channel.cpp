#include "Channel.hpp"

Channel::Channel(const std::string &name): name_(name), userLimit_(100), isInviteOnly_(false), needPassword_(false){
}

Channel::Channel(): name_(){
}

Channel::~Channel(){
}

Channel& Channel::operator=(const Channel &other){
	if (this != &other) {
		name_ = other.name_;
		topic_ = other.topic_;
		clientVector_ = other.clientVector_;
		operatorVector_ = other.operatorVector_;
		userLimit_ = other.userLimit_;
		password_ = other.password_;
		isInviteOnly_ = other.isInviteOnly_;
	}
	return *this;
}

bool Channel::operator!=(const Channel &other){
	return (name_ != other.name_ ? true : false);
}

///@param
// Client client. received by ref that will be added in the Vector that contain the connected client
///@brief
//add client to the list of connected client
void Channel::addClient(Client &client){
	clientVector_.push_back(client);;
}

///@param
// Client client client received by ref that will be removed in the Vector that contain the connected client
///@brief
//remove client to the list of connected client
void Channel::removeClient(const Client &client){
	for (unsigned long i = 0; i < clientVector_.size(); ++i){
		if (client == clientVector_[i]){
			clientVector_.erase(clientVector_.begin() + i);
			return ;
		}
	}
}

///@param
// Client client received by ref that will be added in the Vector that contain the list of operator
///@brief
//add client to the operator for the channel
void Channel::addOperator(Client &client){
	operatorVector_.push_back(client);;
}

///@param
// Client client received by ref that will be removed in the Vector that contain the list of operator
///@brief
//removed Client form the operator Vector for the channel
void Channel::removeOperator(Client &client){
	for (unsigned long i = 0; i < operatorVector_.size(); ++i){
		if (client == operatorVector_[i]){
			operatorVector_.erase(operatorVector_.begin() + i);
			return ;
		}
	}
}

///@brief
//tell if a client is an operator for the current channel
///@param
// client : the client that is checked for operator rights
///@return
//true or false if the client is operator
bool Channel::isOperator(Client &client){
	for (unsigned long i = 0; i < operatorVector_.size(); ++i){
		if (client == operatorVector_[i]){
			return true;
		}
	}
	return false;
}

///@brief
//send a message to every client in the channel
///@param
// msg : message that will be send to every user
void Channel::broadcastEveryone(const std::string &msg){
	for (unsigned long i = 0; i < clientVector_.size(); ++i){
		clientVector_[i].send(msg);
	}
}

///@brief
//send a message to every client other than a specific one in the channel
///@param
// msg : message that will be send to every other user,
///@param
// sender : the Client excluded from the messages,
void Channel::broadcastEveryoneElse(const std::string &msg, Client &sender){
	for (unsigned long i = 0; i < clientVector_.size(); ++i){
		if (sender != clientVector_[i])
			clientVector_[i].send(msg);
	}
}

Client &Channel::getClientByHostName(std::string name){
		for (unsigned long i = 0; i < clientVector_.size(); ++i){
		if (clientVector_[i].getHostName() == name)
			return clientVector_[i];
	}
	throw std::runtime_error("Client not found");//TODO changer pour le code d'erreur

}

Client &Channel::getClientByUserName(std::string name){
	for (unsigned long i = 0; i < clientVector_.size(); ++i){
		if (clientVector_[i].getUserName() == name)
			return clientVector_[i];
	}
	throw std::runtime_error("Client not found");//TODO changer pour le code d'erreur
}

Client &Channel::getClientByNickName(std::string name){
	for (unsigned long i = 0; i < clientVector_.size(); ++i){
		if (clientVector_[i].getNickName() == name)
			return clientVector_[i];
	}
	throw std::runtime_error("Client not found");
}


void Channel::joinChannel(Client &client){
	printf("%d\n", needPassword_);
	if (!needPassword_)
	{
		std::cout << "bravo " << client.getNickName() << " you join join " << name_ << std::endl;
		addClient(client);
		return ;
	}
	throw std::runtime_error("need password");//TODO changer pour le code d'erreur

}

void Channel::joinChannel(Client &client,const std::string &password){
		std::cout << "bravo " << client.getNickName() << " you join channel " << name_ << "with PASSWORD :" << password_ << std::endl;
	if (password == password_) {
		addClient(client);
		return ;
	}
	throw std::runtime_error(ERR_PASSWDMISMATCH(client.getNickName()));
}

void Channel::addInviteList(Client &invite){
		InviteList_.push_back(invite);
}

bool Channel::isOnInviteList(Client &client){
	for (unsigned long i = 0; i < InviteList_.size(); ++i){
		if (client == InviteList_[i])
			return true;
	}
	return false;
}

 void Channel::sendUserList(Client &client) {
	std::string userList;
	for (size_t i = 0; i < clientVector_.size(); i++) {
		userList += clientVector_[i].getNickName();
		if (i != clientVector_.size()-1)
			userList += " ";
	}
	client.send(RPL_NAMREPLY(client.getNickName(), "=", name_, userList));
	client.send(RPL_ENDOFNAMES(client.getNickName(), name_));
 }

 void Channel::broadcastUserList(Client &client) {
	std::string userList;
	for (size_t i = 0; i < clientVector_.size(); i++) {
		userList += clientVector_[i].getNickName();
		if (i != clientVector_.size()-1)
			userList += " ";
	}
	broadcastEveryone(RPL_NAMREPLY(client.getNickName(), "=", name_, userList));
	broadcastEveryone(RPL_ENDOFNAMES(client.getNickName(), name_));
 }