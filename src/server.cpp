#include "server.hpp"

/** ---------- Construct and Destruct ---------- */

Server::Server( void ) {}

Server::~Server( void ) {}

/** ---------- Members ---------- **/

/*
	For each port stored
	- Create a socket
	- Set socket options
	- Bind socket to port
	- Listen on socket
*/
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
		std::memset(_serveraddrs[_serverfds[i]].sin_zero, '\0', sizeof(_serveraddrs[_serverfds[i]].sin_zero));

		if (bind(_serverfds[i], (struct sockaddr *)&_serveraddrs[_serverfds[i]], sizeof(_serveraddrs[_serverfds[i]])) < 0)
			error("bind");

		if (listen(_serverfds[i], 1024) < 0)
			error("listen");
		std::cout << YELLOW << "Initialised port " << GREEN_BOLD << _ports[i] << CLEAR << std::endl;
	}
}

/*
	Accept a connection from a client
	- Accept connection
	- Set socket to non-blocking
	- Add client socket to clientfds
	- Add client address to clientaddrs
	- Add client socket to readfds
*/
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

	std::cout << GREEN << "Accepted new connection on socket " << GREEN_BOLD << clientfd << CLEAR << std::endl;
}

/*
	Read a request from a client
	- Read data from client
	- Process request
	- Add response to corresponding socket
	- Switch client socket from readfds to writefds
*/
void Server::readRequest( int socket, Request &request ) {
	int bytes_read;
	int total_bytes_read = 0;
	int port = -1;
	char buffer[1024];
	std::string client_data;
	std::string host_header = "Host: ";
	size_t host_pos;
	ServerConfig portinfo;
	portinfo.listen = -1;

	while (1) {
		std::memset(buffer, 0, 1024);
		bytes_read = recv(socket, buffer, 1024, 0);

		if (bytes_read < 0) {
			if (errno == EWOULDBLOCK || errno == EAGAIN)
				return ;
			else {
				error("recv", false);
				return closeConnection(socket);
			}
		}
		else if (bytes_read == 0)
			return closeConnection(socket);

		client_data.append(buffer, bytes_read);
		total_bytes_read += bytes_read;

		if (port == -1) {
			host_pos = client_data.find(host_header);
			if (host_pos != std::string::npos) {
				size_t port_pos = host_pos + host_header.length();
				size_t colon_pos = client_data.find(':', port_pos);
				size_t end_pos = client_data.find('\n', port_pos);

				if (colon_pos != std::string::npos && colon_pos < end_pos) {
					std::string port_str = client_data.substr(colon_pos + 1, end_pos - colon_pos - 1);
					port = std::stoi(port_str);
				}
			}
			
			if (port == -1)
				port = 80;
		}

		if (port != -1 || portinfo.listen != port) {
			int connected_port_index = 0;
			for (std::vector<ServerConfig>::iterator iter = configinfo.begin(); iter < configinfo.end(); iter++) {
				if ((*iter).listen == port)
					break ;
				connected_port_index++;
			}

			portinfo = configinfo[connected_port_index];
		}

		if (total_bytes_read >= portinfo.maxClientBodySize) {
			std::cout << RED << "Request exceeds payload limit" << std::endl;

			_response[socket] = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\n\r\n";
			_isparsed[socket] = true;

			FD_CLR(socket, &_readfds);
			FD_SET(socket, &_writefds);
			return ;
		}

		if (bytes_read < 1024)
			break ;
	}

	std::cout << GREEN << "Received " << total_bytes_read << " bytes\n" << CLEAR << std::endl;
	std::cout << client_data << std::endl;

	_response[socket] = request.processRequest(client_data, total_bytes_read, portinfo);
	_isparsed[socket] = true;

	FD_CLR(socket, &_readfds);
	FD_SET(socket, &_writefds);
}

/*
	Send a response to a client
	- Send data to client
	- Close connection if all data has been sent
*/
void Server::sendResponse( int socket ) {
	const char *response = _response[socket].c_str();
	size_t response_len = _response[socket].length();
	size_t total_sent = _sentbytes[socket];

	size_t chunk_size = 1024;
	size_t remaining = response_len - total_sent;

	if (remaining > 0) {
		size_t sentbytes = send(socket, response + total_sent, std::min(chunk_size, remaining), 0);

		if (sentbytes < 0 || errno == EBADF) {
			error("send", false);
            std::cout << "Send failed/bad file descriptor" << std::endl;
			return;
		}

		_sentbytes[socket] += sentbytes;
	}
    // std::cout << errno << std::endl;
	if (_sentbytes[socket] < (int)response_len && errno != EPIPE)
		FD_SET(socket, &_writefds);
	else {
		std::cout << GREEN << "Sent " << _sentbytes[socket] << " bytes" << std::endl;
		closeConnection(socket);
	}
}

/*
	Close a connection with a client
	- Remove all socket information from storage
	- Close socket
*/
void Server::closeConnection( int socket ) {
	_clientfds.erase(std::remove(_clientfds.begin(), _clientfds.end(), socket), _clientfds.end());
	_clientaddrs.erase(socket);
	_response.erase(socket);
	_isparsed.erase(socket);
	_sentbytes.erase(socket);
	close(socket);
	
	std::cout << BLUE << "Closed connection on socket " << socket << "\n" << CLEAR << std::endl;
}

/*
	Main server loop, loops through all stored sockets
	- Add server sockets to readfds
	- Accept connections from clients
	- Read requests from clients
	- Send responses to clients
*/
void Server::loop( void ) {
	fd_set readfds_copy, writefds_copy;
	timeval timeout;
	Request request;

	FD_ZERO(&readfds_copy);
	FD_ZERO(&writefds_copy);
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;

    signal(SIGPIPE, SIG_IGN);
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

/*
	The Server::run( void ) function encompases the
	initialisation and looping of the server
*/
void Server::run( void ) {
	init();
	loop();
}

/** ---------- Error and Exit ---------- **/

void Server::error( std::string errmsg, bool exitbool ) {
	std::cerr << RED << errmsg << ": ";
	perror(NULL);
	std::cerr << CLEAR;

	if (exitbool)
		exit(1);
}

/** ---------- Getters and Setters ---------- **/

void Server::addPort( int port ) {
	_ports.push_back(port);
}