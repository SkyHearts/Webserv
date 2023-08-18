#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

struct s_connection {
	int					server_fd;
	std::vector<int>	client_sockets;
} t_connection;

class Server {
	private:
		std::vector<int>	_ports;
		std::vector<int>	_server_fds;
		std::vector<int>	_client_sockets;
		fd_set				_read_fds;
		int					_max_fd;

		int init( int port );
		void connectionHandler( void );

	public:
		Server( void );
		~Server( void );

		void set_port( int port );
		int run( void );
};