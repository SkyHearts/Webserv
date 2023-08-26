// https://www.geeksforgeeks.org/socket-programming-cc/

#include "server.hpp"

Server::Server( void ) {}

Server::~Server( void ) {}

/*

*/
void Server::init( void ) {
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;

	_serverfds.resize(_serverports.size());
	_serveraddrs.resize(_serverports.size());

	for (size_t i = 0; i < _serverports.size(); i++) {
		if ((_serverfds[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			error("Socket", true);
		std::cout << "Socket: " << _serverfds[i] << std::endl;

		int optval = 1;
		if (setsockopt(_serverfds[i], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
			error("Setsockopt", true);
		std::cout << "Setsockopt reuseaddr to " << optval << std::endl;

		addr.sin_port = htons(_serverports[i]);
		if (bind(_serverfds[i], (struct sockaddr *)&_serveraddrs[i], sizeof(_serveraddrs[i])) == -1)
			error("Bind", true);
		std::cout << "Bound to port " << _serverports[i] << std::endl;

		if (listen(_serverfds[i], SOMAXCONN) == -1)
			error("Listen", true);
		std::cout << "Listening on port " << _serverports[i] << std::endl;
	}
}

void Server::acceptConnection( int fd ) {
	_socket = accept(fd, NULL, NULL);
	if (_socket == -1) {
		error("Accept");
		return ;
	}
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	for (size_t i = 0; i < _serverfds.size(); i++) { // Take note of the server that accepted the connection
		if (fd == _serverfds[i]) {
			_serverindex[_socket] = i;
			break ;
		}
	}
	std::cout << "Accepted connection on server" << _serverfds[_serverindex[_socket]] << std::endl;
	_sentbytes[_socket] = 0;
	_isparsed[_socket] = false;
	_connections++;
	FD_SET(_socket, &_readfds);
}

void Server::handleRequest( void ) {
	char buffer[1024];
	memset(buffer, 0, 1024);

	int request_len = recv(_socket, buffer, 1024, 0);
	if (request_len <= 0) {
		error("Recv");
		_sentbytes[_socket] = 0;
		_buffer[_socket].clear();
		_response[_socket].clear();
		_isparsed.erase(_socket);
		close(_socket);
		FD_CLR(_socket, &_readfds);
		return ;
	}
	std::cout << "Received: " << request_len << " bytes" << std::endl;
	//else if (parseHeader()) {}
	while (request_len > 0) {
		_buffer[_socket].append(buffer);
		std::cout << "Received: " << _buffer[_socket].size() << "/r";
		std::cout.flush();
		memset(buffer, 0, 1024);
		request_len = recv(_socket, buffer, 1024, 0);
	}
	//if (parseHeader()) {}
	FD_CLR(_socket, &_readfds);
	FD_SET(_socket, &_writefds);
}

int Server::parseRequest( void ) {
	// Parse client request into useable format first

	// Temporarily just set response to 200 OK and say Hello World
	_response[_socket] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello World!";
	std::cout << "Set response to client "	<< _socket << std::endl;
	// Error handle once we have a parser
	// Check if client request exceeds max request size here too?
	return 0;
}

void Server::executeRequest( void ) {
	// Execute request here
}

void Server::sendResponse( void ) {
	long total = _sentbytes[_socket];
	long sendbytes = send(_socket, _response[_socket].c_str() + total, _response[_socket].size() - total, 0);
	if (sendbytes <= 0)
		error("Send");
	else {
		_sentbytes[_socket] += sendbytes;
		std::cout << "Sending: " << _sentbytes[_socket] << "/" << _response[_socket].size() << "\r";
		std::cout.flush();
		if ((size_t)_sentbytes[_socket] != _response[_socket].size())
			return ;
		std::cout << std::endl;
		std::cout << "Replied and ";
	}

	_sentbytes.erase(_socket);
	_buffer.erase(_socket);
	_response.erase(_socket);
	_isparsed.erase(_socket);
	
	close(_socket);
	FD_CLR(_socket, &_writefds);
	std::cout << "Closed connection: " << _socket << "\nTotal connections: " << --_connections << std::endl;

	std::cout << "Listening on port(s): ";
	for (size_t i = 0; i < _serverports.size(); i++)
		std::cout << _serverports[i] << " ";
	std::cout << "\nAwaiting connections..." << std::endl;
}

void Server::loop( void ) {
	fd_set readfds, writefds;
	timeval timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);

	for (size_t i = 0; i < _serverfds.size(); i++) {
		_isparsed[_serverfds[i]] = false;
		FD_SET(_serverfds[i], &_readfds);
	}
	_socket = 0;

	std::cout << "Listening on port(s): ";
	for (size_t i = 0; i < _serverports.size(); i++)
		std::cout << _serverports[i] << " ";
	std::cout << "\nAwaiting connections..." << std::endl;

	while (1) {
		memcpy(&readfds, &_readfds, sizeof(_readfds));
		memcpy(&writefds, &_writefds, sizeof(_writefds));
		if (select(FD_SETSIZE, &readfds, &writefds, NULL, &timeout) == 0)
			continue ;

		std::cout << "Someone is trying to connect" << std::endl;

		// Handling reads
		for (int fd = 0; fd < FD_SETSIZE; fd++) {
			std::cout << "Handling reads" << std::endl;
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
				std::cout << "New connection: " << fd << "\nTotal connections: " << _connections << std::endl;
			}
			else {
				_socket = fd;
				handleRequest();
			}
		}

		// Handling writes
		for (int fd = 0; fd < FD_SETSIZE; fd++) {
			std::cout << "Handling writes" << std::endl;
			if (!FD_ISSET(fd, &writefds))
				continue ;

			_socket = fd;
			// Have to add request parsing and actual response sending here later
			bool flag = false;
			for (size_t i = 0; i < FD_SETSIZE; i++) {
				if (_isparsed[i]) {
					flag = true;
					break ;
				}
			}

			if (!_isparsed[_socket] && !flag) {
				std::cout << "Parsing request" << std::endl;
				if (!parseRequest()) {
					std::cout << "Executing request" << std::endl;
					executeRequest();
				}
				_isparsed[_socket] = true;
			}

			if (_isparsed[_socket]) {
				std::cout << "Sending response" << std::endl;
				sendResponse();
			}
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
	_serverports.push_back(8080);
	init();
	std::cout << "Initialized server, starting loop" << std::endl;
	loop();
	return 0;
}