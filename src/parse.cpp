/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:01:43 by jyim              #+#    #+#             */
/*   Updated: 2023/10/10 18:42:13 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse.hpp"

/** ----------- Constructs and Deconstruct ----------- **/

Config::Config( void ) {}

Config::~Config( void ) {}

Config::Config( std::string config_file ) {
	std::ifstream file(config_file);
	std::string serverConfig = Config::getstring(config_file);
	Config::parse(serverConfig);
	Config::checkDupPorts();
}

/*
	If path is directory return true, /test/ = true
	Else return false, /test/test.html = false
*/
static bool isValidDir( const char *path ) {
	struct stat info;

	if (stat(path, &info) == 0)
		return S_ISDIR(info.st_mode);
	else
		return 0;
}

/*
	If path is regular file return true, /test/test.html = true
	Else return false, /test/ = false
*/
static bool isValidFile( const char *path ) {
	struct stat info;

	if (stat(path, &info) == 0)
		return S_ISREG(info.st_mode);
	else
		return 0;
}

/** ---------- Members ---------- **/

/*
	If connected, return false
	Else true
*/
bool Config::CheckPortTCP( short int dwPort, const char *ipAddressStr )
{
	struct sockaddr_in client;		 
	int client_fd, result;   

	client.sin_family = AF_INET;  
	client.sin_port = htons(dwPort);  
	client.sin_addr.s_addr = inet_addr(ipAddressStr);

	if ((client_fd = (int) socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "\nSocket creation error" << std::endl;
		return false;
	}

	if((result = connect(client_fd, (struct sockaddr *) &client,sizeof(client))) == -1)
		return true;
	else {
		close(client_fd);
		return false;
	}
}

/*
	Remove punctuations listed in ispunct(int c) form a string
	e.g. this is a text; -> this is a text
	e.g. this is a text} -> this is a text
*/
void removePunc( std::string& subs ) {
	for (int i = 0, len = subs.size(); i < len; i++) {
		if (is_punct(subs[i])) {
			subs.erase(i--, 1);
			len = subs.size();
		}
	}
}

/*
	Reads and return the full string from a file
*/						  
std::string	Config::getstring( std::string config_file ) {
	std::ifstream fsFile(config_file);
	std::string ret;
	std::string line;

	if (!fsFile) {
		std::cerr << "File not found" << std::endl;
		return ("");
	}

	while(std::getline(fsFile, line))
		ret += line + "\n";

	return ret;
}

/*
	Maps strings to values to enable strings to be used in switch case
*/
void Config::initEnumServerBlock( std::map<std::string, serverBlock> &s_mapStringValues ) {
	s_mapStringValues["listen"] = listenenum;
	s_mapStringValues["server_name"] = server_name;
	s_mapStringValues["root"] = root;
	s_mapStringValues["location"] = location;
	s_mapStringValues["index"] = indexServ;
	s_mapStringValues["error_pages"] = errorPages;
	s_mapStringValues["max_client_body_size"] = max_body_size;
	s_mapStringValues["{"] = start;
	s_mapStringValues["}"] = ended;
}

/*
	Inserts server block server name
*/
void Config::parseServerName( std::istringstream &iss, ServerConfig *server ) {
	std::string subs;
	iss >> subs;

	for (int i = 0, len = subs.size(); i < len; i++) {
		if (is_punct(subs[i])) {
			subs.erase(i--, 1);
			len = subs.size();
		}
	}
	server->name = subs;
}

/*
	Inserts server block listening port
*/
void Config::parseListen( std::istringstream &iss, ServerConfig *server ) {
	int end = 0;
	server->listen = 0;

	do {
		std::string subs;
		iss >> subs;
		if (checkNum(subs))
			throw std::invalid_argument("Numbers only for ports");

		if (subs.find(";") != size_t(-1))
			end = 1;

		if (!server->listen && Config::CheckPortTCP(stoi(subs), "127.0.0.1"))
			server->listen = stoi(subs);
	} while (end == 0);

	if (!server->listen)
		 throw std::invalid_argument("No opened port");
}	

/*
	Inserts server block root
*/
void Config::parseRoot( std::istringstream &iss, ServerConfig *server ) {
	int end = 0;
	int filled = 0;

	do {
		std::string subs;
		iss >> subs;
		if (subs.find(";") != size_t(-1))
			end = 1;

		for (int i = 0, len = subs.size(); i < len; i++) {
			if (is_punct(subs[i])) {
				subs.erase(i--, 1);
				len = subs.size();
			}
   		}

		if (!filled && isValidDir(subs.c_str())) {
			server->root = subs;
			filled = 1;
		}
	} while (end == 0);

	if (!filled)
		throw std::invalid_argument("Not valid root path");
}

/*
	Inserts server block index
*/
void Config::parseIndexServ( std::istringstream &iss, ServerConfig *server ) {
	int end = 0;
	int filled = 0;

	do {
		std::string subs;
		iss >> subs;
		if (subs.find(";") != size_t(-1))
			end = 1;

		removePunc(subs);
		if (!filled && isValidFile((server->root + "/" + subs).c_str())) {
			server->index = subs;
			filled = 1;
		}
		else{
			server->index = subs;
			filled = 1;
		}
	} while (end == 0);

	if (!filled)
		throw std::invalid_argument("Index: Not valid file");
}

/*
	Check word by word and pass to relevant function to fill Location struct similar to parseServerBlock()
*/
void Config::parseLocationParams( std::istringstream &iss, ServerConfig *server, struct Location *loc, std::string& update ) {
	std::string subs;
	int end = 0;

	do {
		iss >> subs;
		if (subs.find("}") != size_t(-1))
			end = 1;

		if (!subs.compare("allowedMethods") && !end) {
			bool stop = false;
			while (!stop) {
				iss >> subs;
				if (subs.find(";") != size_t(-1)) {
					removePunc(subs);
					stop = true;
				}
				loc->allowedMethods.push_back(subs);
			}
		}
		else if ((!subs.compare("index") || !subs.compare("return")) && !end) {
			iss >> subs;

			removePunc(subs);
			if (isValidFile((server->root + "/" + loc->uri + "/" + subs).c_str())){
				loc->index = subs;
				std::cout << "here" <<std::endl;

			}
			else if (subs.find("http") != std::string::npos)
			{
				std::cout << "here http " << subs << std::endl;
				loc->index = subs;
			}
			else
				throw(std::invalid_argument("Invalid Index file path"));
		}	
		else if (!subs.compare("autoindex") && !end) {
			iss >> subs;
			removePunc(subs);
			if (subs == "off") {
				loc->autoindex = false;
			}
			else if(subs == "on") {
				loc->autoindex = true;
			}
			else throw(std::invalid_argument("Invalid autoindex bool"));
		}

	} while (end == 0);

	update = subs;
}

/*
	Inserts index, uri, autoindex and allowedMethods in location blocks using parseLocationParams()
*/
void Config::parseLocation( std::istringstream &iss, ServerConfig *server ) {
	std::string subs;
	struct Location loc;
	loc.autoindex = false;
	int end = 1;
	iss >> subs;

	// if (isValidDir((server->root + subs).c_str()))
		loc.uri = subs;
	// else if (subs.find("http") != std::string::npos)
	// 	loc.uri = subs;
	// else
	// 	throw(std::invalid_argument("No path given"));

	do {
		iss >> subs;
		if (subs.find("{") != size_t(-1))
			end = 0;

		if (subs == "{" && !end)
			Config::parseLocationParams(iss, server, &loc, subs);

		if (subs.find("}") != size_t(-1))
			end = 1;
	} while (end == 0);

	server->locations.push_back(loc);
}

/*
	Parse error page in conf
	Maps them to std::map <errnum(int), path(string)> errorPages
*/
void Config::parseErrorPages( std::istringstream &iss, ServerConfig *server ) {
	int filled = 0;
	int errorNum = 0;
	std::string subs;
	iss >> subs;

	removePunc(subs);
	if (!checkAlpha(subs))
		errorNum = std::stoi(subs);

	iss >> subs;
	removePunc(subs);
	if (errorNum && !filled && isValidFile(subs.c_str())) {
		server->errorPages[errorNum] = subs;
		filled = 1;
	}

	if (!filled)
		throw std::invalid_argument("Not valid error page/Not a number");

}

/*
	Parse max_client_body_size in conf
	Inserts server block max_client_body_size
*/
void Config::parseClientBodySize( std::istringstream &iss, ServerConfig *server ) {
	std::string subs;
	iss >> subs;

	for (int i = 0, len = subs.size(); i < len; i++) {
		if (is_punct(subs[i])) {
			subs.erase(i--, 1);
			len = subs.size();
		}
	}
	server->maxClientBodySize = std::stoi(subs);
}

/* 
	Check if all struct is filled, if not the default path will be filled instead
*/
void	Config::checkDefaults( ServerConfig &server ) {
	if (server.root.empty()) {
		server.root = ROOT_DEFAULT;
		std::cout << server.name << " is using root default" << std::endl;
	}
	if (server.index.empty()) {
		server.index = INDEX_DEFAULT;
		std::cout << server.name << " is using index default" << std::endl;
	}
	if (!server.errorPages.count(404)) {
		server.errorPages[404] = e404_DEFAULT;
		std::cout << server.name << " is using 404 default" << std::endl;
	}
	if (!server.errorPages.count(405)) {
		server.errorPages[405] = e405_DEFAULT;
		std::cout << server.name << " is using 405 default" << std::endl;
	}
	if (!server.errorPages.count(501)) {
		server.errorPages[501] = e501_DEFAULT;
		std::cout << server.name << " is using 501 default" << std::endl;
	}
}

/*
	Go through each word in conf and pass to relevant function to parse variable in ServerConfig struct
*/
void Config::parseServerBlock( std::istringstream &iss ) {
	std::map<std::string, serverBlock> s_mapStringValues;
	Config::initEnumServerBlock(s_mapStringValues);
	struct ServerConfig server;
	int end = 1;
	static int serverBlockIndex = 1;
	server.name = "Serverblock " + std::to_string(serverBlockIndex);
	server.maxClientBodySize = MAXCLIENTBODYSIZE_DEFAULT;

	do {
		std::string subs;
		iss >> subs;
		if (subs.find("{") != size_t(-1))
			end = 0;

		if (subs.find("}") != size_t(-1))
			end = 1;

		switch (s_mapStringValues[subs]) {
			case listenenum:
				parseListen(iss, &server);
				break ;
			case server_name:
				parseServerName(iss, &server);
				break ;
			case root:
				parseRoot(iss, &server);
				break ;
			case location:
				parseLocation(iss, &server);
				break ;
			case indexServ:
				parseIndexServ(iss, &server);
				break ;
			case errorPages:
				parseErrorPages(iss, &server);
				break ;
			case max_body_size:
				parseClientBodySize(iss, &server);
				break ;
			case start:
				break ;
			case ended:
				break ;
			default:
				throw std::invalid_argument("Invalid keyword/format");
		}
	} while (end == 0);
    checkDefaults(server);
	this->_ports.push_back(server);
	serverBlockIndex += 1;
}

/*
	Starts the parsing of conf file
*/
void Config::parse( std::string &file ) {
	std::istringstream iss(file);

	do {
		std::string subs;
		iss >> subs;
 
		if (subs.find("server") != size_t(-1))
			Config::parseServerBlock(iss);
	} while (iss);
}

/*
	Checks for duplicate ports in config file
*/
void Config::checkDupPorts( void ) {
	std::vector<ServerConfig> ports = this->get_servers();
	std::vector<ServerConfig>::iterator iter;
	std::vector<int> tmp_ports;

	for(iter = ports.begin(); iter < ports.end(); iter++) {
		tmp_ports.push_back((*iter).listen);
	}
	if (containsDuplicate(tmp_ports))
		throw std::invalid_argument("Congif file contains duplicate ports");
}

/** ---------- Utility ---------- **/

/*
	If char is ';' or '}' return true
	Else return false;
*/
bool is_punct( int c ) {
	if (c == ';' || c == '{' || c == '}')
		return true;
	return false;	
}

/*
	If string contains Alphabet return false
	Else return true
*/
bool checkAlpha( const std::string &str ) {
	bool retVal = false;
	const char *tmp_str = str.c_str();
	int size = strlen(tmp_str);

	for(int i = 0; i < size; i++) {
		if( (!isalpha(tmp_str[i]) || !isspace(tmp_str[i])) && tmp_str[i] == ';') {
			retVal = true;
			std::cerr << "Input must only contain letters" << std::endl;
			break ;
		}
	}
	return retVal;
}

/*
	If string contains Alphabet return false
	Else return true
*/
bool checkNum( const std::string &str ) {
	bool retVal = false;
	const char *tmp_str = str.c_str();
	int size = strlen(tmp_str);

	for(int i = 0; i < size; i++) {
		if(!isnumber(tmp_str[i]) && tmp_str[i] != ';') {
			retVal = true;
			std::cerr << "Input must only contain numbers"  << std::endl;
			break ;
		}
	}
	return retVal;
}

/*
	If vector of int contains duplicate number return true
	Else return false
*/
bool containsDuplicate( const std::vector<int>& nums ) {
	if(nums.empty())
		return false ;

	for (size_t i = 0; i < nums.size() - 1; ++i)
		for (size_t j = i + 1; j < nums.size(); ++j)
			if (nums[i] == nums[j])
				return true;
	return false;
}