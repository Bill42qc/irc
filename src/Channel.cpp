#include "Channel.hpp"

Channel::Channel(const std::string &name): name_(name){
}

Channel::Channel(): name_(){
}

Channel::~Channel(){
}

///@param
// Client client. received by ref that will be added in the map that contain the connected client
///@brief
//add client to the list of connected client
void Channel::addClient(Client &client){
	clientMap_.insert(std::make_pair(client.getClientSocket(), client));
}

///@param
// Client client client received by ref that will be removed in the map that contain the connected client
///@brief
//remove client to the list of connected client
void Channel::removeClient(const Client &client){
	clientMap_.erase(client.getClientSocket());
}

///@param
// Client client received by ref that will be added in the map that contain the list of operator
///@brief
//add client to the operator for the channel
void Channel::addOperator(Client &client){
	operatorMap_.insert(std::make_pair(client.getClientSocket(), client));
}

///@param
// Client client received by ref that will be removed in the map that contain the list of operator
///@brief
//removed Client form the operator map for the channel
void Channel::removeOperator(Client &client){
	operatorMap_.erase(client.getClientSocket());
}


///@brief
//tell if a client is an operator for the current channel
///@param
// client : the client that is checked for operator rights
///@return
//true or false if the client is operator
bool Channel::isOperator(Client &client){
    std::map<int, Client>::const_iterator it;
    
    for (it = operatorMap_.begin(); it != operatorMap_.end(); ++it) {
        if (it->second.getClientSocket() == client.getClientSocket()) {
            return true; 
        }
    }
    return false;
}

///@brief
//send a message to every client in the channel
///@param
// msg : message that will be send to every user
void Channel::sendToEveryone(const std::string &msg){
	std::map<int, Client>::iterator it;

    for (it = clientMap_.begin(); it != clientMap_.end(); ++it) {
        it->second.send(msg);
    }

}
