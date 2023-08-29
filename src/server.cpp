// https://www.geeksforgeeks.org/socket-programming-cc/

#include "server.hpp"

/* Constructs */

Server::Server( void ) {}

Server::Server( int port ) {
	_port = port;
	_serverfd = socket(AF_INET, SOCK_STREAM, 0);
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(_port);
	_server_addr.sin_addr.s_addr = INADDR_ANY;
}

Server::~Server( void ) {}

/* Member */

int Server::init( void ) {
	if (_serverfd == -1) {
		std::cerr << "error: Socket creation failure" << std::endl;
		return 1;
	}

	int optval = 1;
	if (setsockopt(_serverfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) == -1) {
		std::cerr << "error: Socket configuration failure" << std::endl;
		return 1;
	}

	if (bind(_serverfd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0) {
		std::cerr << "error: Socket bind failure" << std::endl;
		return 1;
	}

	if (listen(_serverfd, 3) < 0) {
		std::cerr << "error: Unable to listen on socket" << std::endl;
		return 1;
	}

	return 0;
}

int Server::run( void ) {
	if (init()) {
		std::cerr << "error: Error encountered while initialising server" << std::endl;
		return 1;
	}

	fd_set read_fds;
	int max_fd = _serverfd;

	static int count;

	while(1) {
		// Use serverfd to initalise list of monitored sockets 
		FD_ZERO(&read_fds);
		FD_SET(_serverfd, &read_fds);
		// For each connected client, add them to the list of monitored sockets
		for (size_t i = 0; i < _client_sockets.size(); i++)
			FD_SET(_client_sockets[i], &read_fds);

		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
			std::cerr << "error: Unable to select socket" << std::endl;
			return 1;
		}

		// Accept any new incoming connections
		if (FD_ISSET(_serverfd, &read_fds)) {
			int new_socket = accept(_serverfd, NULL, NULL);
			if (new_socket < 0) {
				std::cerr << "error: Server unable to accept incoming connection" << std::endl;
				continue;
			}
			count++;
			_client_sockets.push_back(new_socket);
			max_fd = std::max(max_fd, new_socket);
		}

		for (size_t i = 0; i < _client_sockets.size(); i++) {
			int client_socket = _client_sockets[i];
			if (FD_ISSET(client_socket, &read_fds)) {
				char buffer[1024];
				int valread = read(client_socket, buffer, 1024);
				if (valread <= 0) {
					// If no response from client, close connection
					close(client_socket);
					_client_sockets.erase(_client_sockets.begin() + i);
					i--;
				}
				else {
					// Else, send response
					std::string request(buffer, valread);
					std::cout << request << std::endl;

					_req.parseRequest(request);

					std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, world! ";
					response += std::to_string(count);
					response += "</h1></body></html>";
					send(client_socket, response.c_str(), response.length(), 0);
				}
			}
		}
	}
}