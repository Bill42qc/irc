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

/**
	* ERR_NONICKNAMEGIVEN (431)
	* Returned when a nickname parameter is expected for a command but isn’t given.
*/
#define ERR_NONICKNAMEGIVEN(client) ("431 " + client + " " + " :No nickname given" + CRLF)
/**
	* ERR_ERRONEUSNICKNAME (432)
	* Returned when a NICK command cannot be successfully completed as the desired
	* nickname contains characters that are disallowed by the server.
	* See the NICK command for more information on characters which are allowed in various IRC servers.
	* The text used in the last param of this message may vary.
*/
#define ERR_ERRONEUSNICKNAME(client, nick) ("432 " + client + nick + " :Erroneus nickname" + CRLF)
/**
	* ERR_NICKNAMEINUSE (433)
	* Returned when a NICK command cannot be successfully completed as the desired nickname is already in use on the network.
	* The text used in the last param of this message may vary.
*/
#define ERR_NICKNAMEINUSE(client, nick) ("433 " + client + nick + " :Nickname is already in use" + CRLF)
/**
	* ERR_NICKCOLLISION (436)
	* Returned by a server to a client when it detects a nickname collision
	* (registered of a NICK that already exists by another server).
	* The text used in the last param of this message may vary.
*/
#define ERR_NICKCOLLISION(client, nick, user, host) ("436 " + client + nick + " :Nickname collision KILL from" + user + host CRLF)
/**
	*ERR_NEEDMOREPARAMS (461)
	* Returned when a client command cannot be parsed because not enough parameters were supplied.
	* The text used in the last param of this message may vary.
*/
#define ERR_NEEDMOREPARAMS(client, command) ("461 " + client + command + " :Not enough parameters" + CRLF)
/**
	* ERR_ALREADYREGISTERED (462)
	* Returned when a client tries to change a detail that can only be set during registration
	* (such as resending the PASS or USER after registration). The text used in the last param of this message varies.
*/
#define ERR_ALREADYREGISTERED(client) ("462 " + client + command + " :You may not reregister" + CRLF)
/**
	* ERR_PASSWDMISMATCH (464)
	* Returned to indicate that the connection could not be registered as
	* the password was either incorrect or not supplied.
	* The text used in the last param of this message may vary.
*/
#define ERR_PASSWDMISMATCH(client) ("464 " + client + " :Password incorrect" + CRLF)
/**
	* ERR_NOSUCHCHANNEL (403)
	* Indicates that no channel can be found for the supplied channel name.
	* The text used in the last param of this message may vary.
*/
#define ERR_NOSUCHCHANNEL(client, channel) ("403 " + client + channel + " :No such channel" + CRLF)
/**
	* ERR_TOOMANYCHANNELS (405)
	* Indicates that the JOIN command failed because the client has joined their maximum number of channels.
	* The text used in the last param of this message may vary.
*/
#define ERR_TOOMANYCHANNELS(client, channel) ("405 " + client + channel + " :You have joined too many channels" + CRLF)
/**
	* ERR_BADCHANNELKEY (475)
	* Returned to indicate that a JOIN command failed because the channel
	* requires a key and the key was either incorrect or not supplied.
	* The text used in the last param of this message may vary.
*/
#define ERR_BADCHANNELKEY(client, channel) ("475 " + client + channel + " :Cannot join channel (+k)" + CRLF)
/**
	* ERR_BANNEDFROMCHAN (474)
	* Returned to indicate that a JOIN command failed because the client has been banned
	* from the channel and has not had a ban exception set for them.
	* The text used in the last param of this message may vary.
*/
#define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + channel + " :Cannot join channel (+b)" + CRLF)
/**
	* ERR_CHANNELISFULL (471)
	* Returned to indicate that a JOIN command failed because the client limit mode has been set
	* and the maximum number of users are already joined to the channel.
	* The text used in the last param of this message may vary.
*/
#define ERR_CHANNELISFULL(client, channel) ("471 " + client + channel + " :Cannot join channel (+l)" + CRLF)
/**
	* ERR_INVITEONLYCHAN (473)
	* Returned to indicate that a JOIN command failed because the channel is set
	* to [invite-only] mode and the client has not been invited to the channel or had an invite exception set for them.
	* The text used in the last param of this message may vary.
*/
#define ERR_INVITEONLYCHAN(client, channel)  ("473 " + client + channel + " :Cannot join channel (+i)" + CRLF)
/**
	* ERR_BADCHANMASK (476)
	* Indicates the supplied channel name is not a valid.
	* This is similar to, but stronger than, ERR_NOSUCHCHANNEL (403),
	* which indicates that the channel does not exist, but that it may be a valid name.
	* The text used in the last param of this message may vary.
*/
#define ERR_BADCHANMASK(channel) ("476 " + channel + " :Bad Channel Mask" + CRLF)
/**
 * ERR_NOTONCHANNEL (442)
 * Returned when a client tries to perform a channel-affecting command on a channel which the client isn’t a part of.
*/
#define ERR_NOTONCHANNEL(client, nick, channel) ("442 " + client + " " + channel + " :You're not on that channel")

// RPL CORRECT //

/**
	* RPL_TOPIC (332)
	* Sent to a client when joining the <channel> to inform them of the current topic of the channel.
*/
#define RPL_TOPIC(client, channel, topic) ("332 " + client + " " + channel + " :" + topic + CRLF)
/**
	* RPL_TOPICWHOTIME (333)
	* Sent to a client to let them know who set the topic (<nick>) and
	* when they set it (<setat> is a unix timestamp). Sent after RPL_TOPIC (332).
*/
#define RPL_TOPICWHOTIME(client, channel, nick, setat) ("333 " + client + channel + nick + setat + CRLF)
/**
	* RPL_NAMREPLY (353)
	* Sent as a reply to the NAMES command, this numeric lists the clients that are joined to <channel> and their status in that channel.
	* <symbol> notes the status of the channel. It can be one of the following:
	* ("=", 0x3D) - Public channel.
	* ("@", 0x40) - Secret channel (secret channel mode "+s").
	* ("*", 0x2A) - Private channel (was "+p", no longer widely used today).
	* <nick> is the nickname of a client joined to that channel, and <prefix> is the highest channel
	* membership prefix that client has in the channel, if they have one.
	* The last parameter of this numeric is a list of [prefix]<nick> pairs, delimited by a SPACE character (' ', 0x20).
*/
#define RPL_NAMREPLY(client, symbol, channel, prefix, nick) (":irc 353 " + client + " "  + symbol + " " + channel + " :[" + prefix + "]" + nick + "{ [" + prefix + "]" + nick + "}" + CRLF)
/**
	* RPL_ENDOFNAMES (366)
	* Sent as a reply to the NAMES command, this numeric specifies the end of a list of channel member names.
*/
#define RPL_ENDOFNAMES(client, channel) ("366 " + client + channel + " :End of /NAMES list" << CRLF)
/**
	* Join a chanel
*/
#define RPL_JOIN(nick, channel) (":" + nick + " JOIN " + channel + CRLF)
/**
	* set nick name.
*/
#define RPL_NICK(nick, newNick) (":" + nick + " NICK " + newNick + " :" + nick + " has changed their nickname to " + newNick + CRLF)
/**
	* set nick name.
*/
#define RPL_WELCOME(nick, user, host) ("001 " + nick + " :Welcome to the IRC server, " + nick + "!" + user + "@" + host + CRLF)

////Function

std::vector<std::string> splitString(const std::string &input, char delimiter);
void password_check (std::string serverPassword, std::string clientPassword);

#endif