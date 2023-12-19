#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <iostream>
# include <vector>
# include <string>

// COLOR-----------------------------------------------------------------------
# define BCK "\x1B[30m"
# define RED "\x1B[31m"
# define GRE "\x1B[32m"
# define YEL "\x1B[33m"
# define BLE "\x1B[34m"
# define MAG "\x1B[35m"
# define CYA "\x1B[36m"
# define WHT "\x1B[37m"


#define CRLF "\r\n"

// ERROR //

#define ERR_NOSUCHNICK(client, nick) ("401 " + client + " " + nick + " :No such nick/channel" + CRLF)
#define ERR_NOSUCHSERVER(client, serverName) ("402 " + client + " " + serverName + " :No such server" + CRLF)
#define ERR_NOSUCHCHANNEL(client, channel) ("403 " + client + " " + channel + " :No such channel" + CRLF)
#define ERR_CANNOTSENDTOCHAN(client, channel) ("404 " + client + " " + channel + " :Cannot send to channel" + CRLF)
#define ERR_TOOMANYCHANNELS(client, channel) ("405 " + client + channel + " :You have joined too many channels" + CRLF)
#define ERR_NOORIGIN(client) ("409 " + client + " :No origin specified" + CRLF)
#define ERR_NORECIPIENT(client, command) ("411 " + client + " :No recipient given" + command + CRLF)
#define ERR_NOTEXTTOSEND(client) ("412 " + client + " :No text to send" + CRLF)
#define ERR_NONICKNAMEGIVEN(client) ("431 " + client + " " + " :No nickname given" + CRLF)
#define ERR_ERRONEUSNICKNAME(client, nick) ("432 " + client + nick + " :Erroneus nickname" + CRLF)
#define ERR_NICKNAMEINUSE(client, nick) ("433 " + client + nick + " :Nickname is already in use" + CRLF)
#define ERR_NICKCOLLISION(client, nick, user, host) ("436 " + client + nick + " :Nickname collision KILL from" + user + host CRLF)
#define ERR_USERNOTINCHANNEL(client, nick, channel) ("441 " + client + " " + nick + " " + channel + " :They aren't on that channel" + CRLF)
#define ERR_NOTONCHANNEL(client, channel) ("442 " + client + " " + channel + " :You're not on that channel" + CRLF)
#define ERR_USERONCHANNEL(client, nick, channel) ("443 " + client + " " + nick  + " " + channel + " :is already on channel" + CRLF)
#define ERR_NEEDMOREPARAMS(client, command) ("461 " + client + " " + command + " :Not enough parameters" + CRLF)
#define ERR_ALREADYREGISTERED(client) ("462 " + client + command + " :You may not reregister" + CRLF)
#define ERR_PASSWDMISMATCH(client) ("464 " + client + " :Password incorrect" + CRLF)
#define ERR_CHANNELISFULL(client, channel) ("471 " + client + channel + " :Cannot join channel (+l)" + CRLF)
#define ERR_INVITEONLYCHAN(client, channel)  ("473 " + client + channel + " :Cannot join channel (+i)" + CRLF)
#define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + channel + " :Cannot join channel (+b)" + CRLF)
#define ERR_BADCHANNELKEY(client, channel) ("475 " + client + channel + " :Cannot join channel (+k)" + CRLF)
#define ERR_BADCHANMASK(channel) ("476 " + channel + " :Bad Channel Mask" + CRLF)
#define ERR_CHANOPRIVSNEEDED(client, channel) ("482 " + client + " " + channel + " :You're not channel operator" + CRLF)
#define ERR_NOOPERHOST(client) ("491 " + client + " :No O-lines for your host" + CRLF)
#define ERR_INVALIDMODEPARAM(client, channel, char , param, description) ("696 " + client + " " + channel + " " + char + " " + param + " :" + discription + CRLF)
// RPL CORRECT //

#define RPL_WELCOME(nick, user, host) ("001 " + nick + " :Welcome to the IRC server, " + nick + "!" + user + "@" + host + CRLF)
#define RPL_AWAY(client, nick, msg) ("301 " + client + " " + nick + " :Is outline" + CRLF)
#define RPL_CHANNELMODEIS(client, channel, mode) ("324 " + client + " " + channel + " " + mode + CRLF)
#define RPL_TOPIC(client, channel, topic) ("332 " + client + " " + channel + " :" + topic + CRLF)
#define RPL_TOPICWHOTIME(client, channel, nick, setat) ("333 " + client + channel + nick + setat + CRLF)
#define RPL_INVITING(client, nick, channel) ("341 " + client + " " + nick  + " " + channel + CRLF)
#define RPL_NAMREPLY(client, symbol, channel, userList) (":localhost 353 " + client + " "  + symbol + " " + channel + " :" + userList + CRLF)
#define RPL_ENDOFNAMES(client, channel) ("366 " + client + " " + channel + " :End of /NAMES list" + CRLF)
#define RPL_YOUREOPER(client) ("381 " + " :You are now an IRC operator" + CRLF)

#define RPL_NICK(nick, newNick) (":" + nick + " NICK " + newNick + " :" + nick + " changed his nickname to " + newNick + CRLF)
#define RPL_JOIN(nick, channel) (":" + nick + " JOIN " + channel + CRLF)
#define RPL_INVITE(client , newClient, channel) (":" + client +  " INVITE " + newClient + " " + channel + CRLF)
#define RPL_NEWTOPIC(channel, topic) ("TOPIC " + channel + " :" + topic + CRLF)
#define RPL_PART(client, channel) (":" + client + " PART " + channel + CRLF)
#define RPL_MSGONECLIENT(sender, recept, msg) (":" + sender + " PRIVMSG " + recept + " :" + msg + CRLF)
#define	RPL_MSGCHANNEL(sender, channel, msg) (":" + sender + " PRIVMSG " + channel + " :" + msg + CRLF)
#define RPL_KICKED(nick, channel, kickNick, msg) (":" + nick + " KICK " + channel + " " + kickNick + " :" + msg + CRLF)
#define RPL_KICKER(channel, kickNick, msg) (": KICK " + channel + " " + kickNick + " :" + msg + CRLF)

////Function

std::vector<std::string> splitString(const std::string &input, char delimiter);
void password_check (std::string serverPassword, std::string clientPassword);

#endif