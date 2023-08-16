// https://www.geeksforgeeks.org/socket-programming-cc/

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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

	static int count;
	
	// Accept incoming connections
	int addrlen = sizeof(server_addr);
	while (1) {
		int new_socket = accept(serverfd, (struct sockaddr *)&server_addr, (socklen_t*)&addrlen);
		if (new_socket < 0) {
			std::cerr << "Error accepting connection" << std::endl;
			continue;
		}
		count++;

		char buffer[1024];
		int valread = read(new_socket, buffer, 1024);
		std::string request(buffer, valread);
		std::cout << request << std::endl;

		std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, world!</h1></body></html>";
		send(new_socket, response.c_str(), response.length(), 0);
		close(new_socket);
	}

	return 0;
}