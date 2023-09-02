#include "server.hpp"

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

		int optval = 1;
		if (setsockopt(_serverfds[i], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
			error("setsockopt");

		_serveraddrs[_serverfds[i]].sin_family = AF_INET;
		_serveraddrs[_serverfds[i]].sin_addr.s_addr = INADDR_ANY;
		_serveraddrs[_serverfds[i]].sin_port = htons(_ports[i]);
		memset(_serveraddrs[_serverfds[i]].sin_zero, '\0', sizeof(_serveraddrs[_serverfds[i]].sin_zero));

		if (bind(_serverfds[i], (struct sockaddr *)&_serveraddrs[_serverfds[i]], sizeof(_serveraddrs[_serverfds[i]])) < 0)
			error("bind");

		if (listen(_serverfds[i], 10) < 0)
			error("listen");
		std::cout << YELLOW << "Initialised port " << GREEN_BOLD << _ports[i] << CLEAR << std::endl;
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
	_clientfds.push_back(clientfd);
	_clientaddrs[clientfd] = clientaddr;
	FD_SET(clientfd, &_readfds);

	std::cout << GREEN << "Accepted new connection from client fd " << GREEN_BOLD << clientfd << CLEAR << std::endl;
}

void Server::readRequest( int socket, Request &request ) {
	std::cout << YELLOW << "Attempting to read from client " << socket << CLEAR << std::endl;

	char buffer[1024];
	memset(buffer, '\0', 1024);
	int bytes_read = recv(socket, buffer, 1024, 0);

	if (bytes_read < 0) {
		if (errno == EWOULDBLOCK || errno == EAGAIN)
			return ;
		else {
			error("recv", false);
			closeConnection(socket);
			return ;
		}
	}
	else if (bytes_read == 0) {
		std::cout << BLUE << "Client " << socket << " disconnected" << CLEAR << std::endl;
		closeConnection(socket);
		return ;
	}

	std::cout << GREEN << "Received " << bytes_read << " bytes" << std::endl;
	std::cout << CLEAR << "Client says:\n" << buffer << std::endl;

	// std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
	_response[socket] = request.processRequest(buffer);
	_isparsed[socket] = true;

	FD_CLR(socket, &_readfds);
	FD_SET(socket, &_writefds);
}

void Server::sendResponse( int socket ) {
	std::cout << YELLOW << "Attempting to send response to client " << socket << CLEAR << std::endl;

	size_t total_sent = _sentbytes[socket];
	const char *response = _response[socket].c_str();
	size_t response_len = _response[socket].length();

	size_t sentbytes = send(socket, response + total_sent, response_len - total_sent, 0);

	if (sentbytes < 0) {
		error("send", false);
		return ;
	}

	_sentbytes[socket] += sentbytes;
	if ((size_t)_sentbytes[socket] >= response_len) {
		FD_CLR(socket, &_writefds);
		FD_SET(socket, &_readfds);
	}

	std::cout << GREEN << "Sent " << sentbytes << " bytes to client " << socket << "!" << CLEAR << std::endl;
	closeConnection(socket);
}

void Server::closeConnection( int socket ) {
	_clientfds.erase(std::remove(_clientfds.begin(), _clientfds.end(), socket), _clientfds.end());
	_clientaddrs.erase(socket);
	_response.erase(socket);
	_isparsed.erase(socket);
	_sentbytes.erase(socket);
	close(socket);
	
	std::cout << BLUE << "Closed connection on socket " << socket << "\n" << CLEAR << std::endl;
}

void Server::loop( void ) {
	fd_set readfds_copy, writefds_copy;
	timeval timeout;
	Request request;

	FD_ZERO(&readfds_copy);
	FD_ZERO(&writefds_copy);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	for (size_t i = 0; i < _ports.size(); i++)
		FD_SET(_serverfds[i], &_readfds);

	while (1) {
		FD_ZERO(&readfds_copy);
		FD_ZERO(&writefds_copy);
		memcpy(&readfds_copy, &_readfds, sizeof(_readfds));
		memcpy(&writefds_copy, &_writefds, sizeof(_writefds));

		int max_fd = *std::max_element(_serverfds.begin(), _serverfds.end());

		if (select(max_fd + 1, &readfds_copy, &writefds_copy, NULL, &timeout) < 0)
			continue;

		for (size_t i = 0; i < _serverfds.size(); i++)
			if (i < _serverfds.size() && FD_ISSET(_serverfds[i], &readfds_copy))
				acceptConnection(_serverfds[i]);

		for (size_t j = 0; j < _clientfds.size(); j++) {
			FD_ZERO(&readfds_copy);
			memcpy(&readfds_copy, &_readfds, sizeof(_readfds));
			if (j < _clientfds.size() && FD_ISSET(_clientfds[j], &readfds_copy)) {
				readRequest(_clientfds[j], request);
			}

			FD_ZERO(&writefds_copy);
			memcpy(&writefds_copy, &_writefds, sizeof(_writefds));
			if (j < _clientfds.size() && FD_ISSET(_clientfds[j], &writefds_copy) && _isparsed[_clientfds[j]] == true) {
				sendResponse(_clientfds[j]);
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
	std::cerr << RED << errmsg << ": ";
	perror(NULL);
	std::cerr << CLEAR;

	if (exitbool)
		exit(1);
}

/* Getters and Setters */

void Server::addPort( int port ) {
	_ports.push_back(port);
}