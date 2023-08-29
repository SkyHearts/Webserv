#include "headers.hpp"
#include "request.hpp"
class Server {
	private:
		int					_port;
		int					_serverfd;
		struct sockaddr_in	_server_addr;
		std::vector<int>	_client_sockets;
		Request _req;

		Server( void );
		int init( void );

	public:
		Server( int port );
		~Server( void );

		int run( void );
};