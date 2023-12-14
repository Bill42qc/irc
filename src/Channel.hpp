
#pragma once
#include <vector>
#include "Client.hpp"
#include <iostream>
class Client;
class Channel
{
private:
    std::string name_;
    std::string topic_;
    std::string password_;
    std::vector<Client> clientVector_;
    std::vector<Client> operatorVector_;
    std::vector<Client> InviteList_;
    int userLimit_;
    bool isInviteOnly_;
    bool isClientLimited_;
    bool needPassword_;
    Channel();
public:
    // Constructors / Destructor
    Channel(const std::string &name);
    ~Channel();
    Channel& operator=(const Channel &other);
    bool operator!=(const Channel &other);
    // Functions
    void addClient(Client &client);
    void joinChannel(Client &client);
    void joinChannel(Client &client,const std::string &password);
    void removeClient(const Client &client);
    void addOperator(Client &client);
    void removeOperator(Client &client);
    bool isOperator(Client &client);
    bool isOnInviteList(Client &client);
    void broadcastEveryone(const std::string &msg);
    void broadcastEveryoneElse(const std::string &msg, Client &sender);
    void addInviteList(Client &invite);
    Client &getClientByHostName(std::string name);
    Client &getClientByUserName(std::string name);
    Client &getClientByNickName(std::string name);
    //inline function
    void setUserLimit(int limit){userLimit_ = limit;}
    void setInviteOnly(bool flag){isInviteOnly_ = flag;}
    void setPassword(std::string password){password_ = password;}
    void setTopic(std::string topic){topic_ = topic;}
    void setNeedPassword_(bool flag){needPassword_ = flag;}
    void setIsClientLimited_(bool flag){isClientLimited_ = flag;}
    bool getIsClientLimited_(){return isClientLimited_;}
    std::string &getTopic(){return topic_;}
    bool getNeedPassword(){return needPassword_;}
    bool getIsInviteOnly(){return isInviteOnly_;}
    bool isChannelFull();
    bool enterPassword(std::string password){return (password == password_ ? true : false);}
    std::string &getName() {return name_;}
    bool operator<(const Channel& other) const {return (name_ < other.name_);}//overload so it can be use for map
    bool operator==(const std::string &str){return (name_ == str ? true : false);}
};
// Channel_HPP