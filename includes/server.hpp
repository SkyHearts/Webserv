#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

typedef struct s_connection {
	int					port;
	int					server_fd;
	struct sockaddr_in	address;
	std::vector<int>	client_sockets;
	fd_set				read_fd;
	int					max_fd;
} t_connection;

class Server {
	private:
		std::vector<int>			_input_ports;
		std::vector<t_connection>	_server_ports;

		int init( t_connection *new_port );
		void connectionHandler( void );

	public:
		Server( void );
		~Server( void );

		void set_port( int port );
		int run( void );
};