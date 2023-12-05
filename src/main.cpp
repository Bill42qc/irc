#include "ACommand.hpp"
#include "Server.hpp"



int main(int argc, char **argv)
{
	Server server;

	if (argc != 3){
		std::cerr << "./ircserv <port> <password>" << std::endl;
		return 1;
	}
	try{
		server.init(argv[1], argv[2]);	
		server.run();
		server.shutdown();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
}