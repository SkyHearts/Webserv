// https://www.geeksforgeeks.org/socket-programming-cc/

#include "server.hpp"

Server::Server( void ) {}

Server::~Server( void ) {}

/*

*/
void Server::init( void ) {
	sockaddr_in addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;

	_serverfds.resize(_serverports.size());
	_serveraddrs.resize(_serverports.size());

	for (size_t i = 0; i < _serverports.size(); i++) {
		if ((_serverfds[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			error("Socket");

		int optval = 1;
		if (setsockopt(_serverfds[i], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
			error("Setsockopt");

		addr.sin_port = htons(_serverports[i]);
		if (bind(_serverfds[i], (struct sockaddr *)&_serveraddrs[i], sizeof(_serveraddrs[i])) == -1)
			error("Bind");

		if (listen(_serverfds[i], SOMAXCONN) == -1)
			error("Listen");
	}
}

void Server::acceptConnection( int fd ) {
	_socket = accept(fd, NULL, NULL);
	if (_socket == -1) {
		error("Accept");
		return ;
	}
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	for (size_t i = 0; i < _serverfds.size(); i++) {
		if (fd == _serverfds[i]) {
			_serveraddrs[i].sin_port = htons(_serverports[i]);
			_serveraddrs[i].sin_family = AF_INET;
			_serveraddrs[i].sin_addr.s_addr = INADDR_ANY;
			break ;
		}
	}
}

void Server::loop( void ) {
	fd_set readfds, writefds;
	timeval timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);

	for (size_t i = 0; i < _serverfds.size(); i++)
		FD_SET(_serverfds[i], &_readfds);
	_socket = 0;

	std::cout << "Listening on ports: ";
	for (size_t i = 0; i < _serverports.size(); i++)
		std::cout << _serverports[i] << " ";
	std::cout << "Awaiting connections..." << std::endl;

	static int connections;
	while (1) {
		memcpy(&readfds, &_readfds, sizeof(_readfds));
		memcpy(&writefds, &_writefds, sizeof(_writefds));
		if (select(FD_SETSIZE, &readfds, &writefds, NULL, &timeout) == 0)
			continue ;

		// Handling reads
		for (int fd = 0; fd < FD_SETSIZE; fd++) {
			if (!FD_ISSET(fd, &readfds))
				continue ;
			
			bool is_server = false;
			for (size_t i = 0; i < _serverfds.size(); i++) {
				if (fd == _serverfds[i]) {
					is_server = true;
					break ;
				}
			}

			if (is_server) {
				acceptConnection(fd);
				std::cout << "New connection: " << fd << std::endl;
			}
			else {
				_socket = fd;
				handleRequest();
			}
		}

		// Handling writes
		for (int fd = 0; fd < FD_SETSIZE; fd++) {
			if (!FD_ISSET(fd, &writefds))
				continue ;

			_socket = fd;
			// Have to add request parsing and actual response sending here later
			sendResponse();
		}
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