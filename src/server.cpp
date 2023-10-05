/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 13:47:30 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/05 17:28:03 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void Server::init(void) {
	for (size_t i = 0; i < this->_ports.size(); i++) {
		int serverfd = socket(AF_INET, SOCK_STREAM, 0);
		if (serverfd < 0)
			error("socket", true);
		this->_serverfds.push_back(serverfd);

		int optval = 1;
		if (setsockopt(this->_serverfds[i], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
			error("setsockopt", true);

		struct addrinfo hints, *serverinfo;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		if (getaddrinfo(NULL, std::to_string(this->_ports[i]).c_str(), &hints, &serverinfo) != 0)
			error("getaddrinfo", true);

		if (bind(this->_serverfds[i], serverinfo->ai_addr, serverinfo->ai_addrlen) < 0)
			error("bind", true);

		if (listen(this->_serverfds[i], 1024) < 0)
			error("listen", true);

		freeaddrinfo(serverinfo);

		std::cout << YELLOW << "Initialized port " << GREEN_BOLD << this->_ports[i] << CLEAR << std::endl;
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

	this->_clientfds.push_back(clientfd);
	this->_clientaddrs[clientfd] = clientaddr;
	FD_SET(clientfd, &_readfds);

	std::cout << GREEN << "Accepted new connection on socket " << GREEN_BOLD << clientfd << CLEAR << std::endl;
}

/*
	Read a request from a client
	- Read data from client
	- Take note of which port client is connected to
	- Process request with limitations of current port
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
			error("recv", false);
			return closeConnection(socket);
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

		if (port != -1 && portinfo.listen != port) {
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

			this->_response[socket] = std::string(PTL_413) + std::string(PTL_MESSAGE);
			this->_isparsed[socket] = true;

			FD_CLR(socket, &this->_readfds);
			FD_SET(socket, &this->_writefds);
			return ;
		}

		if (bytes_read < 1024)
			break ;
	}

	std::cout << GREEN << "Received " << total_bytes_read << " bytes\n" << CLEAR << std::endl;
	std::cout << "[" << client_data << "]" << std::endl;

	this->_response[socket] = request.processRequest(client_data, portinfo);
	this->_isparsed[socket] = true;


	FD_CLR(socket, &this->_readfds);
	FD_SET(socket, &this->_writefds);
}

/*
	Send a response to a client
	- Send data to client
	- Close connection if all data has been sent
*/
void Server::sendResponse( int socket ) {
	const char *response = this->_response[socket].c_str();
	// std::cout << _response[socket] << std::endl;
	size_t response_len = this->_response[socket].length();
	size_t total_sent = this->_sentbytes[socket];
	int sentbytes = 0;

	size_t chunk_size = 1024;
	size_t remaining = response_len - total_sent;

	if (remaining > 0) {
		sentbytes = send(socket, response + total_sent, std::min(chunk_size, remaining), 0);

		if (sentbytes < 0) {
			error("send", false);
			closeConnection(socket);
			return;
		}

		this->_sentbytes[socket] += sentbytes;
	}

	if (this->_sentbytes[socket] == (int)response_len) {
		std::cout << GREEN << "Sent " << this->_sentbytes[socket] << " bytes" << std::endl;
		closeConnection(socket);
	}

}

/*
	Close a connection with a client
	- Remove all socket information from storage
	- Close socket
*/
void Server::closeConnection( int socket ) {
	this->_clientfds.erase(std::remove(this->_clientfds.begin(), this->_clientfds.end(), socket), this->_clientfds.end());
	this->_clientaddrs.erase(socket);
	this->_response.erase(socket);
	this->_isparsed.erase(socket);
	this->_sentbytes.erase(socket);
	close(socket);
	
	std::cout << BLUE << "Closed connection on socket " << socket << "\n" << CLEAR << std::endl;
}

/*
	Main server loop, loops through all stored sockets
	- Add server sockets to readfds
	- Accept connections from clients
	- Read requests from clients
	- Send responses to clients

	listens for incoming connections and handles them
	It sets up file descriptors for reading and writing,
	sets a timeout, and waits for incoming connections using
	the select() function. When a connection is accepted,
	it reads the request from the client, processes it, and
	sends a response back. This loop continues indefinitely until the
	server is stopped.
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
	for (size_t i = 0; i < this->_ports.size(); i++)
		FD_SET(this->_serverfds[i], &this->_readfds);

	while (1) {
		FD_ZERO(&readfds_copy);
		FD_ZERO(&writefds_copy);
		memcpy(&readfds_copy, &this->_readfds, sizeof(this->_readfds));
		memcpy(&writefds_copy, &this->_writefds, sizeof(this->_writefds));

		int max_fd = *std::max_element(this->_serverfds.begin(), this->_serverfds.end());

		if (select(max_fd + 1, &readfds_copy, &writefds_copy, NULL, &timeout) < 0)
			continue;

		for (size_t i = 0; i < this->_serverfds.size(); i++)
			if (i < this->_serverfds.size() && FD_ISSET(this->_serverfds[i], &readfds_copy))
				acceptConnection(this->_serverfds[i]);

		for (size_t j = 0; j < this->_clientfds.size(); j++) {
			FD_ZERO(&readfds_copy);
			memcpy(&readfds_copy, &this->_readfds, sizeof(this->_readfds));
			if (j < this->_clientfds.size() && FD_ISSET(this->_clientfds[j], &readfds_copy))
				readRequest(this->_clientfds[j], request);

			FD_ZERO(&writefds_copy);
			memcpy(&writefds_copy, &_writefds, sizeof(this->_writefds));
			if (j < this->_clientfds.size() && FD_ISSET(this->_clientfds[j], &writefds_copy) && this->_isparsed[_clientfds[j]] == true)
				sendResponse(this->_clientfds[j]);
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
	this->_ports.push_back(port);
}