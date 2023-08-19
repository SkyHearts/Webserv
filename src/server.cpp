// https://www.geeksforgeeks.org/socket-programming-cc/

#include "server.hpp"

/** CONSTRUCTS **/

Server::Server( void ) {}

Server::~Server( void ) {
	for (size_t i = 0; i < _server_ports.size(); i++)
		for (size_t j = 0; j < _server_ports[i].client_sockets.size(); j++)
			close(_server_ports[i].client_sockets[j]);
}

/** MEMBER FUNCTIONS **/

/*
	Given a port number, Server::init() initialises a socket on that
	port and returns the file descriptor of the socket.
*/
int init( t_connection *new_port ) {
	new_port->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (new_port->server_fd == -1) {
		std::cerr << "Error: Socket creation failure" << std::endl;
		return 1;
	}

	int optval = 1;
	if (setsockopt(new_port->server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
		std::cerr << "Error: Socket configuration failure" << std::endl;
		return 1;
	}

	if (bind(new_port->server_fd, (struct sockaddr *)&new_port->address, sizeof(new_port->address)) < 0) {
		std::cerr << "Error: Socket bind failure" << std::endl;
		return 1;
	}

	if (listen(new_port->server_fd, 3) < 0) {
		std::cerr << "Error: Unable to listen on socket" << std::endl;
		return 1;
	}

	return 0;
}

void Server::connectionHandler( void ) {
	static int count;

	while (1) {
		for (size_t i = 0; i < _server_ports.size(); i++) {
			FD_ZERO(&_server_ports[i].read_fd);
			FD_SET(_server_ports[i].server_fd, &_server_ports[i].read_fd);
			for (size_t j = 0; j < _server_ports[i].client_sockets.size(); j++)
				FD_SET(_server_ports[i].client_sockets[j], &_server_ports[i].read_fd);

			if (select(_server_ports[i].max_fd + 1, &_server_ports[i].read_fd, NULL, NULL, NULL) == -1) {
				std::cerr << "Error: Unable to select socket" << std::endl;
				return;
			}

			if (FD_ISSET(_server_ports[i].server_fd, &_server_ports[i].read_fd)) {
				int new_socket = accept(_server_ports[i].server_fd, NULL, NULL);
				if (new_socket < 0) {
					std::cerr << "Error: Unable to accept connection" << std::endl;
					continue;
				}
				_server_ports[i].client_sockets.push_back(new_socket);
				_server_ports[i].max_fd = std::max(_server_ports[i].max_fd, new_socket);
			}

			for (size_t i = 0; i < _server_ports.size(); i++)
				for (size_t j = 0; j < _server_ports[i].client_sockets.size(); j++)
					if (FD_ISSET(_server_ports[i].client_sockets[j], &_server_ports[i].read_fd)) {
						char buffer[1024] = {0};
						int valread = read(_server_ports[i].client_sockets[j], buffer, 1024);
						if (valread == 0) {
							close(_server_ports[i].client_sockets[j]);
							_server_ports[i].client_sockets.erase(_server_ports[i].client_sockets.begin() + j);
						}
						else
							std::cout << buffer << std::endl;
						count++;
						std::cout << count << std::endl;
					}
				}
			}
		}
	}
}

int Server::run( void ) {
	for (size_t i = 0; i < _input_ports.size(); i++) {
		t_connection new_port;
		new_port.port = _input_ports[i];
		new_port.address.sin_family = AF_INET;
		new_port.address.sin_port = htons(_input_ports[i]);
		new_port.address.sin_addr.s_addr = INADDR_ANY;
		
		if (init(&new_port) == 1) {
			std::cerr << "Error: Unable to initialise server on port " << _input_ports[i] << std::endl;
			return 1;
		}
		if (new_port.server_fd > new_port.max_fd)
			new_port.max_fd = new_port.server_fd;
		
		_server_ports.push_back(new_port);
	}

	connectionHandler();
	return 0;
}

/** GETTER AND SETTER **/
void Server::set_port( int port ) {
	_input_ports.push_back(port);
}