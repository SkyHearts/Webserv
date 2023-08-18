// https://www.geeksforgeeks.org/socket-programming-cc/

#include "server.hpp"

/** CONSTRUCTS **/

Server::Server( void ) {
	FD_ZERO(&_read_fds);
	_max_fd = -1;
}

Server::~Server( void ) {
	for (size_t i = 0; i < _server_fds.size(); i++)
		close(_server_fds[i]);
}

/** MEMBER FUNCTIONS **/

/*
	Given a port number, Server::init() initialises a socket on that
	port and returns the file descriptor of the socket.
*/
int Server::init( int port ) {
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		std::cerr << "Error: Socket creation failure" << std::endl;
		return -1;
	}

	int optval = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
		std::cerr << "Error: Socket configuration failure" << std::endl;
		return -1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Error: Socket bind failure" << std::endl;
		return -1;
	}

	if (listen(server_fd, 3) < 0) {
		std::cerr << "Error: Unable to listen on socket" << std::endl;
		return -1;
	}

	return server_fd;
}

void Server::connectionHandler( void ) {
	fd_set read_fds;

	static int count;

	while (1) {
		FD_ZERO(&read_fds);
		for (size_t i = 0; i < _server_fds.size(); i++)
			FD_SET(_server_fds[i], &read_fds);
		for (size_t i = 0; i < _client_sockets.size(); i++)
			FD_SET(_client_sockets[i], &read_fds);

		if (select(_max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
			std::cerr << "Error: Select failure" << std::endl;
			return;
		}

		for (size_t i = 0; i < _server_fds.size(); i++) {
			if (FD_ISSET(_server_fds[i], &read_fds)) {
				int new_socket = accept(_server_fds[i], NULL, NULL);
				if (new_socket < 0) {
					std::cerr << "Error: Unable to accept connection" << std::endl;
					return;
				}

				_client_sockets.push_back(new_socket);
				if (new_socket > _max_fd)
					_max_fd = new_socket;
			}
		}

		for (size_t i = 0; i < _client_sockets.size(); i++) {
			int client_socket = _client_sockets[i];
			if (FD_ISSET(client_socket, &read_fds)) {
				char request[1024];
				int valread = read(client_socket, request, 1024);
				if (valread == 0) {
					close(client_socket);
					_client_sockets.erase(_client_sockets.begin() + i);
					i--;
				}
				else {
					std::cout << request << std::endl;
					std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, world! ";
                    response += std::to_string(count);
                    response += "</h1></body></html>";
                    send(client_socket, response.c_str(), response.length(), 0);
					count++;
				}
			}
		}
	}
}

int Server::run( void ) {
	int server_fd;

	for (size_t i = 0; i < _ports.size(); i++) {
		server_fd = init(_ports[i]);
		if (server_fd == -1) {
			std::cerr << "Error: Unable to initialise server on port " << _ports[i] << std::endl;
			return 1;
		}
		
		_server_fds.push_back(server_fd);
		if (server_fd > _max_fd)
			_max_fd = server_fd;
	}

	connectionHandler();
	return 0;
}

/** GETTER AND SETTER **/
void Server::set_port( int port ) {
	_ports.push_back(port);
}