#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <iostream>
# include <vector>
# include <string>

#define CRLF "\r\n"

// NICK ERROR
/**
	* ERR_NONICKNAMEGIVEN (431)
	* Returned when a nickname parameter is expected for a command but isn’t given.
*/
#define ERR_NONICKNAMEGIVEN (client) client  << " :No nickname given" << CRLF
/**
	* ERR_ERRONEUSNICKNAME (432)
	* Returned when a NICK command cannot be successfully completed as the desired
	* nickname contains characters that are disallowed by the server.
	* See the NICK command for more information on characters which are allowed in various IRC servers.
	* The text used in the last param of this message may vary.
*/
#define ERR_ERRONEUSNICKNAME (client, nick) client << nick << " :Erroneus nickname" << CRLF
/**
	* ERR_NICKNAMEINUSE (433)
	* Returned when a NICK command cannot be successfully completed as the desired nickname is already in use on the network.
	* The text used in the last param of this message may vary.
*/
#define ERR_NICKNAMEINUSE (client, nick) client << nick << " :Nickname is already in use" << CRLF
/**
	* ERR_NICKCOLLISION (436)
	* Returned by a server to a client when it detects a nickname collision
	* (registered of a NICK that already exists by another server).
	* The text used in the last param of this message may vary.
*/
#define ERR_NICKCOLLISION (client, nick, user, host) client << nick << " :Nickname collision KILL from" << user << host CRLF

// USER ERROR
/**
	*ERR_NEEDMOREPARAMS (461)
	* Returned when a client command cannot be parsed because not enough parameters were supplied.
	* The text used in the last param of this message may vary.
*/
#define ERR_NEEDMOREPARAMS (client, command) client << command << " :Not enough parameters" << CRLF
/**
	* ERR_ALREADYREGISTERED (462)
	* Returned when a client tries to change a detail that can only be set during registration
	* (such as resending the PASS or USER after registration). The text used in the last param of this message varies.
*/
#define ERR_ALREADYREGISTERED (client) client << command << " :You may not reregister" << CRLF










std::vector<std::string> splitString(const std::string &input, char delimiter);

#endif