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

typedef struct Port {
	int			port;
	std::string	name;
}				Port;

class Server {
	private:
		fd_set						_readfds, _writefds;
		std::vector<int>			_serverports;
		std::vector<int>			_serverfds;
		std::vector<sockaddr_in>	_serveraddrs;
		Port						_port_info;


		Server( void );
		int init( void );
		int loop( void );

		void error( std::string errmsg, bool exitbool = false );

	public:
		Server( int port );
		~Server( void );

		int run( void );
};