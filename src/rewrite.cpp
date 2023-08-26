#include "rewrite.hpp"

/* Construct and Destruct */

Server::Server( void ) {}

Server::~Server( void ) {}

/* Members */

void Server::init( void ) {
	for (size_t i = 0; i < _ports.size(); i++) {
		int serverfd = socket(AF_INET, SOCK_STREAM, 0);
		if (serverfd < 0)
			error("socket");
		_serverfds.push_back(serverfd);
		std::cout << "Created socket for port " << _ports[i] << std::endl;

		int optval = 1;
		if (setsockopt(_serverfds[i], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
			error("setsockopt");
		std::cout << "Set socket options for port " << _ports[i] << std::endl;

		_serveraddrs[_ports[i]].sin_family = AF_INET;
		_serveraddrs[_ports[i]].sin_addr.s_addr = INADDR_ANY;
		_serveraddrs[_ports[i]].sin_port = htons(_ports[i]);
		memset(_serveraddrs[_ports[i]].sin_zero, '\0', sizeof(_serveraddrs[_ports[i]].sin_zero));

		if (bind(_serverfds[i], (struct sockaddr *)&_serveraddrs[_ports[i]], sizeof(_serveraddrs[_ports[i]])) < 0)
			error("bind");
		std::cout << "Bound to port " << _ports[i] << std::endl;

		if (listen(_serverfds[i], 10) < 0)
			error("listen");
		std::cout << "Listening on port " << _ports[i] << std::endl;
	}
}

void Server::acceptConnection( int serverfd ) {
	sockaddr_in clientaddr;
	socklen_t clientaddrlen = sizeof(clientaddr);
	
	int clientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
	if (clientfd < 0) {
		error("accept", false);
		return ;
	}

	fcntl(clientfd, F_SETFL, O_NONBLOCK);
	_sockets[clientfd] = clientaddr;
	FD_SET(clientfd, &_readfds);

	std::cout << "Accepted connection on port " << ntohs(clientaddr.sin_port) << std::endl;
}

void Server::readRequest( int socket ) {
	std::cout << "In readRequest" << std::endl;

	char buffer[1024];
	int bytes_read = read(socket, buffer, 1024);
	if (bytes_read < 0) {
		error("read", false);
		return ;
	}
	if (bytes_read == 0) {
		closeConnection(socket);
		return ;
	}

	std::cout << "Received " << bytes_read << " bytes\nClient says:" << std::endl;
	std::cout << buffer << std::endl;

	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
	_response[socket] = response;
	_isparsed[socket] = true;

	FD_CLR(socket, &_readfds);
	FD_SET(socket, &_writefds);
}

void Server::sendResponse( int socket ) {
	std::cout << "In sendResponse" << std::endl;

	if (!_response[socket].empty()) {
		size_t total_sent = _sentbytes[socket];
		const char *response = _response[socket].c_str();
		size_t response_len = _response[socket].length();

		size_t sentbytes = send(socket, response + total_sent, response_len - total_sent, 0);

		if (sentbytes < 0) {
			error("send", false);
			closeConnection(socket);
			return ;
		}

		_sentbytes[socket] += sentbytes;
		if ((size_t)_sentbytes[socket] >= response_len) {
			FD_CLR(socket, &_writefds);
			FD_SET(socket, &_readfds);
		}
	}
}

void Server::closeConnection( int socket ) {
	close(socket);
	FD_CLR(socket, &_readfds);
	FD_CLR(socket, &_writefds);
	_sockets.erase(socket);
	_response.erase(socket);
	_isparsed.erase(socket);
	_sentbytes.erase(socket);
	
	std::cout << "Closed connection on socket " << socket << std::endl;
}

void Server::loop( void ) {
	fd_set readfds, writefds;
	timeval timeout;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	for (size_t i = 0; i < _ports.size(); i++)
		FD_SET(_serverfds[i], &_readfds);

	while (1) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		memcpy(&readfds, &_readfds, sizeof(_readfds));
		memcpy(&writefds, &_writefds, sizeof(_writefds));

		int max_fd = *std::max_element(_serverfds.begin(), _serverfds.end());

		if (select(max_fd + 1, &readfds, &writefds, NULL, &timeout) < 0)
			continue;

		for (size_t i = 0; i < _serverfds.size(); i++)
			if (FD_ISSET(_serverfds[i], &readfds))
				acceptConnection(_serverfds[i]);

		for (int fd = 0; fd < FD_SETSIZE; fd++) {
			if (FD_ISSET(fd, &readfds)) {
				std::cout << fd << std::endl;
				readRequest(fd);
			}
			if (FD_ISSET(fd, &writefds) && _isparsed[fd] == true) {
				std::cout << fd << std::endl;
				sendResponse(fd);
			}
		}
	}
}

void Server::run( void ) {
	init();
	loop();
}

/* Error and Exit */

void Server::error( std::string errmsg, bool exitbool ) {
	std::cerr << errmsg << ": ";
	perror(NULL);
	std::cerr << std::endl;

	if (exitbool)
		exit(1);
}

/* Getters and Setters */

void Server::addPort( int port ) {
	_ports.push_back(port);
}