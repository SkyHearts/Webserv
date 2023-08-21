#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

class Server {
	private:
		int					_port;
		int					_serverfd;
		struct sockaddr_in	_server_addr;
		std::vector<int>	_client_sockets;

		Server( void );
		int init( void );

	public:
		Server( int port );
		~Server( void );

		int run( void );
};