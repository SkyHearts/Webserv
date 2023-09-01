#include "server.hpp"
#include <fstream>

int parse_config( std::string config_file ) {
	std::string extension = ".conf";
	if (config_file.length() < extension.length() || config_file.compare(config_file.length() - extension.length(), extension.length(), extension)) {
		std::cerr << RED << "Error: Invalid or missing .conf extension" << CLEAR << std::endl;
		return 1;
	}
	
	std::ifstream file(config_file.c_str());
	if (!file.is_open()) {
		std::cerr << RED << "Error: Unable to open configuration file" << CLEAR << std::endl;
		return 1;
	}

	return 0;
}

int main( int ac, char **av ) {
	Server server;

	if (ac < 0 || ac > 2) {
		std::cerr << RED << "Error: Invalid usage\nUsage: ./webserv [file.conf]" << CLEAR << std::endl;
		return 1;
	}

	std::string config_file = "server.conf";
	if (ac == 2)
		config_file = av[1];

	if (parse_config(config_file))
		return 1;

	// Add ports to listen on
	server.addPort(8080);
	server.addPort(8888);

	server.run();

	return 0;
}