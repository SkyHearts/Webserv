/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:00:42 by jyim              #+#    #+#             */
/*   Updated: 2023/08/18 18:22:43 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <fstream>

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

//server {  //start of server block
//    listen       <port>; // can insert multiple ports
//    server_name  <name>
//    root         <path_to_files>; // e.g. user/documents/

//    location ~ <url_path> { //e.g. /, /cgi-bin, /etc
//     allowedMethods <security>; //restrict what a url is able to send to server. https://coldbox.ortusbooks.com/the-basics/event-handlers/http-method-security
//} //end of server block

struct Location
{
	std::string					path;
	std::vector<std::string>	allowedMethods;
	std::string					root;
	
};


struct ServerConfig
{
	std::vector<int>	listen;
	std::string			name;
	std::string			root;
	std::vector<Location>	locations;
};

class Config {
	private:
		std::vector<ServerConfig>	_servers;
	public:
		Config( void );
		Config(char *av);
		~Config( void );

		void parse(std::fstream &file);
		std::vector<ServerConfig> get_servers(void) { return _servers; }
		std::string	getstring(std::fstream &file);
};