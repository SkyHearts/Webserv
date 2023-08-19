#include "server.hpp"
#include <fstream>

int parse_config( std::string config_file ) {
	std::string extension = ".conf";
	if (config_file.length() < extension.length() || config_file.compare(config_file.length() - extension.length(), extension.length(), extension)) {
		std::cerr << "Error: Invalid or missing .conf extension" << std::endl;
		return 1;
	}

	std::ifstream file(config_file.c_str());
	if (!file.is_open()) {
		std::cerr << "Error: Unable to open configuration file" << std::endl;
		return 1;
	}

	return 0;
}

int main( int ac, char **av ) {
	if (ac == 1) {
		std::cerr << "Error: No configuration file specified\nUsage: ./webserv [file.conf]" << std::endl;
		return 1;
	}

	std::string config_file = av[1];
	if (parse_config(config_file))
		return 1;

	// replace this with config file parsing
	std::cout << av[1] << std::endl;

	// run server on port 80
	Server server;
	server.set_port(80);
	server.set_port(8080);
	server.run();
	return 0;
}