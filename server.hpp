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
		std::vector<int>			_serverfds, _serverports;
		std::vector<sockaddr_in>	_serveraddrs;

		int							_socket, _connections;
		std::map<int, int>			_serverindex, _sentbytes;
		std::map<int, std::string>	_buffer, _response;
		std::map<int, bool>			_isparsed;

		void init( void );
		void acceptConnection( int fd );
		void handleRequest( void );
		void sendResponse( void );
		int parseRequest( void );
		void executeRequest( void );
		void loop( void );

		void error( std::string errmsg, bool exitbool = false );

	public:
		Server( void );
		~Server( void );

		int run( void );
};