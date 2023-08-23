// https://www.geeksforgeeks.org/socket-programming-cc/

#include "server.hpp"

Server::Server( void ) {}

Server::~Server( void ) {}

/*

*/
int Server::init( void ) {
	_serverfds.resize(_serverports.size());
	_serveraddrs.resize(_serverports.size());

	for (size_t i = 0; i < _serverports.size(); i++) {
		if ((_serverfds[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			error("Socket");

		int optval = 1;
		if (setsockopt(_serverfds[i], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
			error("Setsockopt");

		

	}
}

void Server::error( std::string errmsg, bool exitbool ) {
	std::cerr << errmsg << ": ";
	perror(NULL);
	std::cerr << std::endl;

	if (exitbool)
		exit(1);
}

int Server::run( void ) {
	try {
		init();
		loop();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}