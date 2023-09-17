#pragma once

#include "headers.hpp"
#include "request.hpp"

class Server {
	private:
		fd_set						_readfds, _writefds;
		std::vector<int>			_ports, _serverfds, _clientfds;
		std::map<int, sockaddr_in>	_serveraddrs, _clientaddrs;
		std::map<int, std::string>	_response;
		std::map<int, bool>			_isparsed;
		std::map<int, long>			_sentbytes;

		void init( void );
		void acceptConnection( int serverfd );
		void readRequest( int socket, Request &request );
		void sendResponse( int socket );
		void closeConnection( int socket );
		void loop( void );

		void error( std::string errmsg, bool exitbool = true );

	public:
		std::vector<ServerConfig>	configinfo;

		Server();
		~Server();

		void run( void );

		void addPort( int port );
};