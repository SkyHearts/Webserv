/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:00:42 by jyim              #+#    #+#             */
/*   Updated: 2023/08/21 17:23:38 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <fstream>
#include <map>

//class Server {
//	private:
//		int					_port;
//		int					_serverfd;
//		struct sockaddr_in	_server_addr;
//		std::vector<int>	_client_sockets;

//		Server( void );
//		int init( void );

//	public:
//		Server( int port );
//		~Server( void );

//		int run( void );
//};

//server {  //start of server block
//    listen       <port>; // can insert multiple ports
//    server_name  <name>
//    root         <path_to_files>; // e.g. user/documents/

//    location ~ <url_path> { //e.g. /, /cgi-bin, /etc
//     allowedMethods <security>; //restrict what a url is able to send to server. https://coldbox.ortusbooks.com/the-basics/event-handlers/http-method-security
//} //end of server block

//static enum StringValue { evNotDefined,
//                          evStringValue1,
//                          evStringValue2,
//                          evStringValue3,
//                          evEnd };
						  
struct Location
{
	std::string					path;
	std::vector<std::string>	allowedMethods;
	
};

struct ServerConfig
{
	int					listen;
	std::string			name;
	std::string			root;
	std::vector<Location>	locations;
};

class Config {
	private:
		std::vector<ServerConfig>	_ports;
	public:
		Config( void );
		Config(char *av);
		~Config( void );
		enum	serverBlock{
			notDefined = 0,
			listen = 1,
			server_name = 2,
			root = 3,
			location = 4
		};
		void	initEnum(std::map<std::string, serverBlock> &s_mapStringValues);
		void	parse(std::string &file);
		void	parseServerBlock(std::istringstream &iss);
		//void	parseListen(std::istringstream &iss);
		//void	parseServerName(std::istringstream &iss);
		//void	parseRoot(std::istringstream &iss);
		//void	parseLocation(std::istringstream &iss);
		//std::vector<ServerConfig> get_servers(void) { return _ports; }
		std::string	getstring(char *av);
		//void	printfServerConfig(void);
};