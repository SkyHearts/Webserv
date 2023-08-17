// https://www.geeksforgeeks.org/socket-programming-cc/

#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

int main ( int ac, char **av ) {
	// AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd == -1) {
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}

	/*
		Setting the SO_REUSEADDR option on a socket allows the socket to bind
		to a local address that is already in use by another socket. This is
		useful in situations where a server needs to restart or bind to a new
		address while connections are still active on the old address.

		Without SO_REUSEADDR, the old socket would remain in the TIME_WAIT
		state for a period of time after it is closed, preventing the new
		socket from binding to the same address. By setting SO_REUSEADDR,
		the old socket is immediately released and the new socket can bind to the address.
	*/
	int optval = 1;
	if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) == -1) {
		std::cerr << "Error setting socket options" << std::endl;
		return 1;
	}

	// Bind socket to port 80 (HTTP)
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Error binding socket" << std::endl;
		return 1;
	}

	// Listen for connections
	if (listen(serverfd, 3) < 0) {
		std::cerr << "Error listening on socket" << std::endl;
		return 1;
	}

	// Set up array to store client sockets
	std::vector<int> client_sockets;
	fd_set read_fds;
	int max_fd = serverfd;

	static int count;
	
	// Accept incoming connections and sending a response
	while (1) {
		// Use serverfd to initalise list of monitored sockets 
		FD_ZERO(&read_fds);
		FD_SET(serverfd, &read_fds);
		// For each connected client, add them to the list of monitored sockets
		for (size_t i = 0; i < client_sockets.size(); i++)
			FD_SET(client_sockets[i], &read_fds);

		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
			std::cerr << "Error selecting socket" << std::endl;
			return 1;
		}

		// Accept any new incoming connections
		if (FD_ISSET(serverfd, &read_fds)) {
			int new_socket = accept(serverfd, NULL, NULL);
			if (new_socket < 0) {
				std::cerr << "Error accepting connection" << std::endl;
				continue;
			}
			count++;
			client_sockets.push_back(new_socket);
			max_fd = std::max(max_fd, new_socket);
		}

		for (size_t i = 0; i < client_sockets.size(); i++) {
			int client_socket = client_sockets[i];
			if (FD_ISSET(client_socket, &read_fds)) {
				char buffer[1024];
				int valread = read(client_socket, buffer, 1024);
				if (valread <= 0) {
					// If no response from client, close connection
					close(client_socket);
					client_sockets.erase(client_sockets.begin() + i);
					i--;
				}
				else {
					// Else, send response
					std::string request(buffer, valread);
					std::cout << request << std::endl;

					std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, world! ";
					response += std::to_string(count);
					response += "</h1></body></html>";
					send(client_socket, response.c_str(), response.length(), 0);
				}
			}
		}
	}
	
	return 0;
}