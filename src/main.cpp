#include "ACommand.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
	Server server;

	(void) argc;
	(void) argv;
	try
	{
		server.init();
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}