#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>

class Server {
	private:
		fd_set						_readfds, _writefds;
		std::vector<int>			_ports, _serverfds, _clientfds;
		std::map<int, sockaddr_in>	_serveraddrs, _clientaddrs;
		std::map<int, std::string>	_response;
		std::map<int, bool>			_isparsed;
		std::map<int, int>			_sentbytes;

		void init( void );
		void acceptConnection( int serverfd );
		void readRequest( int socket );
		void sendResponse( int socket );
		void closeConnection( int socket );
		void loop( void );

		void error( std::string errmsg, bool exitbool = true );

	public:
		Server();
		~Server();

		void run( void );

		void addPort( int port );
};