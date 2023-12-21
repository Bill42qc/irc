#include "Server.hpp"

bool isStringOnlyDigits(const std::string& str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isdigit(*it)) {
            return false;  // Found a non-digit character
        }
    }
    return true;  // All characters are digits
}

int main(int argc, char **argv)
{
	Server server;
	if (argc != 3){
		std::cerr << "./ircserv <port> <password>" << std::endl;
		return 1;
	}
	std::string port = argv[1];
	if (!(isStringOnlyDigits(port))){
		std::cerr << "port must be numbers only" << std::endl;
		return 1;
	}
	try{
		if(std::stoi(port) > 49151 || std::stoi(port)  < 1024){
			std::cerr << "port out of range" << std::endl;
			return 1;
		}
		server.init(argv[1], argv[2]);
		server.run();
		server.shutdown();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
}