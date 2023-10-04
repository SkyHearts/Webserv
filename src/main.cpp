#include "server.hpp"
#include "parse.hpp"
#include <fstream>

/*
	Checks if the config file has a .conf extension
*/
static int isValidExtension( std::string config_file ) {
	std::string extension = ".conf";
	if (config_file.length() < extension.length() || config_file.compare(config_file.length() - extension.length(), extension.length(), extension)) {
		std::cerr << RED << "Error: Invalid or missing .conf extension" << CLEAR << std::endl;
		return 1;
	}

	return 0;
}

int main( int ac, char **av ) {
	if (ac < 0 || ac > 2) {
		std::cerr << RED << "Error: Invalid usage\nUsage: ./webserv [file.conf]" << CLEAR << std::endl;
		return 1;
	}

	std::string config_file = "server.conf";
	if (ac == 2)
		config_file = av[1];
	else
		std::cout << "Using default config file: server.conf" << std::endl;

	if (isValidExtension(config_file))
		return 1;

	try
	{
		Server server;

		Config S(config_file);
		server.configinfo = S.get_servers();
		for (std::vector<ServerConfig>::iterator iter = server.configinfo.begin(); iter < server.configinfo.end(); iter++)
			server.addPort((*iter).listen);

		server.run();
	}
	catch (std::invalid_argument& e)
	{
        std::cerr << "Config error:" << e.what() << std::endl;
        return -1;
    }

	return 0;
}