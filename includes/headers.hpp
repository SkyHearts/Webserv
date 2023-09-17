#pragma once

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <netdb.h>
#include <fcntl.h>
#include <vector>
#include <cctype>
#include <map>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define GREEN_BOLD "\033[32;1m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CLEAR "\033[0m"

#define HOST "Host"
#define CON "Connection"
#define UIR "Upgrade-Insecure-Requests"
#define UA "User-Agent"
#define ACCEPT "Accept"

enum serverBlock {
	notDefined = 0,
	listenenum = 1,
	server_name = 2,
	root = 3,
	location = 4,
	indexServ = 5,
	errorPages = 6,
	start = 7
};

struct Location {
	std::string					uri;
	std::string 				index;
	std::vector<std::string>	allowedMethods;
	bool autoindex;
};

struct ServerConfig {
	int							listen;
	std::string					name;
	std::string					root;
	std::vector<Location>		locations;
	std::string 				index;
	std::map<int, std::string>	errorPages;
};
