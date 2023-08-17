#include "server.hpp"

int main( int ac, char **av ) {
	if (ac == 1) {
		std::cerr << "error: No configuration file specified\nUsage: ./webserv [file.conf]" << std::endl;
		return 1;
	}
	std::cout << av[1] << std::endl;

	Server server(80);
	server.run();
	return 0;
}