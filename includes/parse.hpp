/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:00:42 by jyim              #+#    #+#             */
/*   Updated: 2023/09/02 17:22:37 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "server.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>

#include <map>
#include <stdexcept>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

// Enum declare
//enum	serverBlock{
//	notDefined = 0,
//	listen = 1,
//	server_name = 2,
//	root = 3,
//	location = 4,
//	start = 5
//};

//enum	parseelocation{
//	notDefined = 0,
//	listen = 1,
//	server_name = 2,
//	root = 3,
//	location = 4,
//	start = 5
//};

struct Location
{
	std::string					uri;
	std::string 				index;
	std::vector<std::string>	allowedMethods;
	bool autoindex;
};

struct ServerConfig
{
	int							listen;
	std::string					name;
	std::string					root;
	std::vector<Location>		locations;
	std::string 				index;
	std::map<int, std::string>	errorPages;
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
			location = 4,
			indexServ = 5,
			errorPages = 6,
			start = 7
		};
		// Check_if_port_occupied
		bool CheckPortTCP(short int dwPort, const char *ipAddressStr);
		// Parse Server Block
		void	initEnumServerBlock(std::map<std::string, serverBlock> &s_mapStringValues);
		void	parse(std::string &file);
		void	parseServerBlock(std::istringstream &iss);
		void	parseListen(std::istringstream &iss, ServerConfig *server);
		void	parseServerName(std::istringstream &iss, ServerConfig *server);
		void	parseRoot(std::istringstream &iss, ServerConfig *server);
		void	parseIndexServ(std::istringstream &iss, ServerConfig *server);
		// Parse Location Block
		void	parseLocation(std::istringstream &iss, ServerConfig *server);
		void	parseLocationParams(std::istringstream &iss, ServerConfig *server, struct Location *loc, std::string& update);
		// Parse Error_Pages
		void	parseErrorPages(std::istringstream &iss, ServerConfig *server);
		
		std::vector<ServerConfig> get_servers(void) { return _ports; }
		std::string	getstring(char *av);
		//void	printfServerConfig(void);
};

// Utils
bool	is_punct(int c);
bool	checkAlpha(const std::string &str);
bool	checkNum(const std::string &str);
bool	isValidDir(const char *path);
bool	isValidFile(const char *path);