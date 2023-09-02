/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:01:43 by jyim              #+#    #+#             */
/*   Updated: 2023/09/02 17:38:52 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse.hpp"
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <cctype>

//#include <map>
//#include <stdexcept>
//#include <sys/stat.h>
//#include <stdlib.h>
//#include <dirent.h>
//#include <iostream>

// if char is ';' or '}' return true
// else retuen false;
bool	is_punct(int c)
{
	if (c == ';' || c == '{' || c == '}')
		return true;
	return false;	
}

// if string contains Alphabet return true
// else return false
bool checkAlpha(const std::string &str){
    bool retVal = false;
    for(int i = 0; i < str.size(); i++){
        if( (!isalpha(str[i]) || !isspace(str[i])) && str[i] == ';'){
            retVal = true;
            std::cout << "Input must only contain letters\n";
            break;
        }
    }
    return retVal;
}

// if path is directory return true, /test/ = true
// else return false, /test/test.html = false
bool	isValidDir(const char *path){
    struct stat info;
	std::cout << "isvaliddir path = " << path << std::endl;
	if (stat(path, &info) == 0)
	{
		if (S_ISDIR(info.st_mode))
			std::cout << path << " is a directory" << std::endl;
		else 
			std::cout << path << " is not a directory" << std::endl;
        return (S_ISDIR(info.st_mode));
	}
    else
        return (0);
}


// if path is regular file return true, /test/test.html = true
// else return false, /test/ = false
bool	isValidFile(const char *path){

    struct stat info;
	std::cout << "isvalidfile path = " << path << std::endl;
	if (stat(path, &info) == 0)
	{
		if (S_ISREG(info.st_mode))
			std::cout << path << " is a regular file" << std::endl;
		else 
			std::cout << path << " is not a regular file" << std::endl;
        return (S_ISREG(info.st_mode));
	}
    else
        return (0);
}

// if string contains Alphabet return true
// else return false
bool checkNum(const std::string &str){
    bool retVal = false;
    for(int i = 0; i < str.size(); i++){
        if( !isnumber(str[i]) && str[i] != ';'){
            retVal = true;
            std::cout << "Input must only contain numbers\n";
            break;
        }
    }
    return retVal;
}

//If connected, return false
//else true
bool Config::CheckPortTCP(short int dwPort, const char *ipAddressStr)
{
	struct sockaddr_in client;         
	int client_fd, result;   

	client.sin_family = AF_INET;  
	client.sin_port = htons(dwPort);  
	client.sin_addr.s_addr = inet_addr(ipAddressStr);
	//client.sin_addr.s_addr = INADDR_ANY;
	if ((client_fd = (int) socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
        return false;
	}  
	if((result = connect(client_fd, (struct sockaddr *) &client,sizeof(client))) == -1)
		return true;
	else
	{
		close(client_fd);
		return false;
	}
}

//remove punctuations listed in ispunct(int c) form a string
//e.g. this is a text; -> this is a text
//e.g. this is a text} -> this is a text
void removePunc(std::string& subs){
	for (int i = 0, len = subs.size(); i < len; i++)
	{
		// check whether parsing character is punctuation or not
		if (is_punct(subs[i]))
		{
			subs.erase(i--, 1);
			len = subs.size();
		}
	}
}

Config::Config(){
	//USE DEFAULT CONF FILE if there are no files specified
	std::cout << "Default constructor" << std::endl;
}

Config::~Config( void ){
	std::cout << "Deconstruct Config" << std::endl;
}

//reads and return the full string from a file						  
std::string	Config::getstring(char *av){
	std::ifstream fsFile(av);
	std::string ret;
	if (!fsFile){
		std::cout << "File not found" << std::endl;
		return ("");
	}
	std::string line;
	while(std::getline(fsFile, line)) {
		std::cout << line << '\n';
		ret += line + "\n";
	}
	return ret;
}

//Maps strings to values to enable strings to be used in switch case
void	Config::initEnumServerBlock(std::map<std::string, serverBlock> &s_mapStringValues){
	s_mapStringValues["listen"] = listen;
	s_mapStringValues["server_name"] = server_name;
	s_mapStringValues["root"] = root;
	s_mapStringValues["location"] = location;
	s_mapStringValues["index"] = indexServ;
	s_mapStringValues["error_pages"] = errorPages;
	s_mapStringValues["{"] = start;
}

void	Config::parseServerName(std::istringstream &iss, ServerConfig *server){
	std::cout << "IN CONFIG::PARSESERVERNAME" << std::endl;
	std::string subs;
	iss >> subs;
	// Print the word fetched
        // from the istringstream
	for (int i = 0, len = subs.size(); i < len; i++)
    {
        // check whether parsing character is punctuation or not
        if (is_punct(subs[i]))
        {
            subs.erase(i--, 1);
            len = subs.size();
        }
    }
    std::cout << "(Start_Name)" << subs << "(End_Name)" << std::endl;
	server->name = subs;
	std::cout << "Server Name is " << server->name <<std::endl;
	std::cout << "EXIT CONFIG::PARSESERVERNAME" << std::endl << std::endl;
}

void	Config::parseListen(std::istringstream &iss, ServerConfig *server){
	std::cout << "IN CONFIG::PARSELISTEN" << std::endl;
	int end = 0;
	//std::cout << "Before:ServerListen: " << server->listen << std::endl;
	server->listen = 0;
	//std::cout << "After:ServerListen: " << server->listen << std::endl;
	do {
		std::string subs;
		iss >> subs;
		if (checkNum(subs))
			throw std::invalid_argument("Numbers only for ports");
		if (subs.find(";") != -1){
			std::cout << "Semicolon found" << std::endl;
			end = 1;
		}
		// Print the word fetched from the istringstream
		std::cout << "(Start_Port)" << subs << "(End_Port)" << std::endl;
		if (!server->listen && Config::CheckPortTCP(stoi(subs), "127.0.0.1")){
			server->listen = stoi(subs);
			std::cout << "Port " << server->listen << " is open" <<std::endl;
		}
	} while (end == 0);
	if (!server->listen)
		 throw std::invalid_argument("No opened port");
	std::cout << "EXIT CONFIG::PARSELISTEN" << std::endl << std::endl;
}	

void	Config::parseRoot(std::istringstream &iss, ServerConfig *server){
	std::cout << "IN CONFIG::PARSEROOT" << std::endl;
	int end = 0;
	int filled = 0;
	do {
		std::string subs;
		iss >> subs;
		if (subs.find(";") != -1){
			std::cout << "Semicolon found" << std::endl;
			end = 1;
		}
		for (int i = 0, len = subs.size(); i < len; i++)
    	{
			// check whether parsing character is punctuation or not
			if (is_punct(subs[i]))
			{
				subs.erase(i--, 1);
				len = subs.size();
        	}
   		}
		// Print the word fetched from the istringstream
		std::cout << "(Start_Root)" << subs << "(End_Root)" << std::endl;
		if (!filled && isValidDir(subs.c_str())){
			server->root = subs;
			filled = 1;
			std::cout << "Root path is " << server->root << std::endl;
		}
	} while (end == 0);
	if (!filled)
		throw std::invalid_argument("Not valid root path");
	std::cout << "EXIT CONFIG::PARSEROOT" << std::endl << std::endl;
}

void	Config::parseIndexServ(std::istringstream &iss, ServerConfig *server){
	std::cout << "IN CONFIG::parseIndexServ" << std::endl;
	int end = 0;
	int filled = 0;
	do {
		std::string subs;
		iss >> subs;
		if (subs.find(";") != -1){
			std::cout << "Semicolon found" << std::endl;
			end = 1;
		}
		removePunc(subs);
		// Print the word fetched from the istringstream
		std::cout << "(Start_Root)" << subs << "(End_Root)" << std::endl;
		if (!filled && isValidFile((server->root + "/" + subs).c_str())){
			server->index = subs;
			filled = 1;
			std::cout << "Index file path is " << server->index << std::endl;
		}
	} while (end == 0);
	if (!filled)
		throw std::invalid_argument("Index: Not valid file");
	std::cout << "EXIT CONFIG::parseIndexServ" << std::endl << std::endl;
}

void	Config::parseLocationParams(std::istringstream &iss, ServerConfig *server, struct Location *loc, std::string& update){
	std::cout << "IN CONFIG::PARSELOCATIONPARAMS" << std::endl;
	std::string subs;
	int end = 0;
	do {
		iss >> subs;
		std::cout << "(Start2)" << subs << "(End2)" << std::endl;
		if (subs.find("}") != -1){
			std::cout << "Exit location params found" << std::endl;
			end = 1;
		}
		if (!subs.compare("allowedMethods") && !end){
			std::cout << "In allowedMethods" << std::endl;
			bool stop = false;
			while (!stop){
				iss >> subs;
				if (subs.find(";") != -1){
					std::cout << "Semicolon found" << std::endl;
					removePunc(subs);
					stop = true;
				}
				std::cout << "(pushback_methods)" << subs << "(pushback_methods)" << std::endl;
				loc->allowedMethods.push_back(subs);
			}
		}
		else if (!subs.compare("index") && !end){
			std::cout << "In LocIndex" << std::endl;
			iss >> subs;
			if (subs.find(";") != -1){
				std::cout << "Semicolon found" << std::endl;
			}
			removePunc(subs);
			if (isValidFile((server->root + "/" + loc->uri + "/" + subs).c_str())){
				loc->index = subs;
				std::cout << "Index file path is " << loc->index << std::endl;
			}
			else throw(std::invalid_argument("Invalid Index file path"));
		}	
		else if (!subs.compare("autoindex") && !end){
			std::cout << "In LocIndex" << std::endl;
			iss >> subs;
			removePunc(subs);
			if (subs == "off"){
				loc->autoindex = false;
			}
			else if(subs == "on"){
				loc->autoindex = true;
			}
			else throw(std::invalid_argument("Invalid autoindex bool"));
		}
		// Print the word fetched from the istringstream
		std::cout << "(inParseLocParams)" << subs << "(inParseLocParams)" << std::endl;
	} while (end == 0);
	update = subs;
	std::cout << "EXIT CONFIG::PARSELOCATIONPARAMS" << std::endl;
}

//Inserts index and allowedMethods in location blocks
void	Config::parseLocation(std::istringstream &iss, ServerConfig *server){
	std::cout << "IN CONFIG::PARSELOCATION" << std::endl;
	std::string subs;
	struct Location loc;
	loc.autoindex = false;//default value as false
	int end = 1;
	int filled = 0;
	iss >> subs;
	std::cout << "(Start2)" << subs << "(End2)" << std::endl;
	if (isValidDir((server->root + subs).c_str()))
	{
		loc.uri = subs;
		std::cout << "Location uri: " << loc.uri << std::endl;
	}
	else throw(std::invalid_argument("No path given"));
	do {
		iss >> subs;
		std::cout << "(Start2)" << subs << "(End2)" << std::endl;
		if (subs.find("{") != -1){
			std::cout << "start location block found" << std::endl;
			end = 0;
		}
		if (subs == "{" && !end){
			Config::parseLocationParams(iss, server, &loc, subs);
			std::cout << "(afterlocparams)" << subs << "(afterlocparams)" << std::endl;
		}
		// Print the word fetched from the istringstream
		std::cout << "(inParseLoc)" << subs << "(inParseLoc)" << std::endl;
		if (subs.find("}") != -1){
			std::cout << "Exit location block found" << std::endl;
			end = 1;
		}
	} while (end == 0);
	//if (!filled)
	//	throw std::invalid_argument("Not valid root path");
	std::cout << "EXIT CONFIG::PARSELOCATION" << std::endl << std::endl;
	server->locations.push_back(loc);
}

void	Config::parseErrorPages(std::istringstream &iss, ServerConfig *server){
	std::cout << "IN CONFIG::PARSEErrorPages" << std::endl;
	int filled = 0;
	int errorNum = 0;
	std::string subs;
	iss >> subs;
	removePunc(subs);
	if (!checkAlpha(subs))
		errorNum = std::stoi(subs);
	iss >> subs;
	removePunc(subs);
	if (errorNum && !filled && isValidFile(subs.c_str())){
		server->errorPages[errorNum] = subs;
		filled = 1;
	}
	if (!filled)
		throw std::invalid_argument("Not valid error page/Not a number");
	std::cout << "EXIT CONFIG::PARSEErrorPages" << std::endl << std::endl;
}

void	Config::parseServerBlock(std::istringstream &iss){
	std::map<std::string, serverBlock> s_mapStringValues;
	Config::initEnumServerBlock(s_mapStringValues);
	struct ServerConfig server;
	int end = 1;
	//try{
	std::cout << "IN CONFIG::PARSESERVERBLOCK" << std::endl;
	do {
    	std::string subs;
        // Get the word from the istringstream
        iss >> subs;
        // Print the word fetched
        // from the istringstream
        std::cout << "(Start2)" << subs << "(End2)" << std::endl;
		// exit server block
		if (subs.find("{") != -1){
			std::cout << "Start Bracket found" << std::endl;
			end = 0;
		}
		if (subs.find("}") != -1){
			std::cout << "Close Bracket found" << std::endl;
			end = 1;
		}
		// parse to correct variable
		switch (s_mapStringValues[subs]){
		case listen:
			std::cout << "Parse Listen" << std::endl;
			parseListen(iss, &server);
			break ;
		case server_name:
			std::cout << "Server Name" << std::endl;
			parseServerName(iss, &server);
			break ;
		case root:
			std::cout << "Parse root" << std::endl;
			parseRoot(iss, &server);
			break ;
		case location:
			std::cout << "Parse location" << std::endl;
			parseLocation(iss, &server);
			break ;
		case indexServ:
			std::cout << "Parse Serverblock index" << std::endl;
			parseIndexServ(iss, &server);
			break ;
		case errorPages:
			std::cout << "Parse Serverblock index" << std::endl;
			parseErrorPages(iss, &server);
			break ;
		default:
			std::cout << "Next word" << std::endl;
		}	
 
    } while (end == 0);
	this->_ports.push_back(server);
	std::cout << "EXIT CONFIG::PARSESERVERBLOCK" << std::endl << std::endl;
}

void	Config::parse(std::string &file){
	//if (file.empty())
	std::istringstream iss(file);
	do {
		std::cout << "IN CONFIG::PARSE" << std::endl;
    	std::string subs;
        // Get the word from the istringstream
        iss >> subs;
 
        // Print the word fetched
        // from the istringstream
        std::cout << "(Start)" << subs << "(End)" << std::endl;

		// parse to correct variable
		if (subs.find("server") != -1)
			Config::parseServerBlock(iss);
 
    } while (iss);
}

Config::Config(char *av){
	std::ifstream file(av);
	std::cout << "File: " << av << std::endl;
	std::string serverConfig = Config::getstring(av);
	std::cout << std::endl;
	Config::parse(serverConfig);
}


int main(int argc, char **argv){
	try // or write in parseServerBlock
	{
		Config S(argv[1]);
		std::cout << std::endl << std::endl << std::endl;
		
		std::vector<ServerConfig> ports = S.get_servers();
		int ports_size = ports.size();
		std::vector<ServerConfig>::iterator iter;
		for(iter = ports.begin(); iter < ports.end(); iter++)
		{
			std::cout << "Server {" << std::endl;
			std::cout << iter->name << std::endl;
			std::cout << "Listen: " << (*iter).listen << std::endl;
			std::cout << "Root: " << (*iter).root << std::endl;
			std::cout << "Index: " << (*iter).index << std::endl;
			if(!(*iter).errorPages.empty()){
				std::cout << "KEY\tELEMENT\n";
				for (std::map<int, std::string>::iterator itr = (*iter).errorPages.begin(); itr != (*iter).errorPages.end(); ++itr) {
					std::cout << itr->first << '\t' << itr->second << '\n';
				}
			}
			std::cout << std::endl;
			if (!(*iter).locations.empty())
			{
				for (std::vector<Location>::iterator loc = (*iter).locations.begin(); loc < (*iter).locations.end(); loc++)
				{
					if (!(*loc).uri.empty())
						std::cout << "location uri: " << (*loc).uri << std::endl;
					if (!(*loc).index.empty())
						std::cout << "location index: " << (*loc).index << std::endl;
					std::cout << "location autoindex: " << (*loc).autoindex << std::endl;
					if (!(*loc).allowedMethods.empty()){
						std::cout << "allowedMethods: ";
						for (std::vector<std::string>::iterator allow = (*loc).allowedMethods.begin(); allow < (*loc).allowedMethods.end(); allow++)
						{
							std::cout << (*allow) << " ";
						}
						std::cout << std::endl;
					}
					std::cout << std::endl;
				}
			}
			std::cout << "}" << std::endl;
		}
		std::cout << std::endl;
		//if (!S.get_servers().empty())
		//	 ;//free servers
	}
	catch (std::invalid_argument& e)
	{
        std::cerr << "Config error:" << e.what() << std::endl;
			//write free here?
        return -1;
		//exit(1);
    }
}