#include "ACommand.hpp"
#include "Server.hpp"



int main(int argc, char **argv)
{
	Server server;

	if (argc != 3){
		std::cerr << "ft irc only need 2 arg" << std::endl;
		return 1;
	}
	try{
		server.init(argv[1], argv[2]);	
		server.run();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
}