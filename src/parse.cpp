/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:01:43 by jyim              #+#    #+#             */
/*   Updated: 2023/08/22 19:57:36 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>

// if char is ';' or '}' return true
// else retuen false;
bool	is_punct(int c)
{
	if (c == ';' || c == '}')
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

bool	isValidDir(const char *path){

    struct stat info;
	if (stat(path, &info) == 0)
        return (1);
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

Config::Config(){
	//USE DEFAULT CONF FILE if there are no files specified
	std::cout << "Default constructor" << std::endl;
}

Config::~Config( void ){
	std::cout << "Deconstruct Config" << std::endl;
}
						  
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

void	Config::initEnum(std::map<std::string, serverBlock> &s_mapStringValues){
	s_mapStringValues["listen"] = listen;
	s_mapStringValues["server_name"] = server_name;
	s_mapStringValues["root"] = root;
	s_mapStringValues["location"] = location;
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
	server->listen = 0;
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

//In progress
void	Config::parseLocation(std::istringstream &iss, ServerConfig *server){
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
		// Print the word fetched from the istringstream
		std::cout << "(Start_Root)" << subs << "(End_Root)" << std::endl;
	} while (end == 0);
	if (!filled)
		throw std::invalid_argument("Not valid root path");
	std::cout << "EXIT CONFIG::PARSEROOT" << std::endl << std::endl;
}

void	Config::parseServerBlock(std::istringstream &iss){
	std::map<std::string, serverBlock> s_mapStringValues;
	Config::initEnum(s_mapStringValues);
	ServerConfig *server = new ServerConfig;
	int end = 0;

	std::cout << "IN CONFIG::PARSESERVERBLOCK" << std::endl;
	do {
    	std::string subs;
        // Get the word from the istringstream
        iss >> subs;
        // Print the word fetched
        // from the istringstream
        std::cout << "(Start2)" << subs << "(End2)" << std::endl;
		// exit server block
		if (subs.find("}") != -1){
			std::cout << "Close Bracket found" << std::endl;
			end = 1;
		}
		// parse to correct variable
		switch (s_mapStringValues[subs]){
		case listen:
			std::cout << "Parse Listen" << std::endl;
			parseListen(iss, server);
			break ;
		case server_name:
			std::cout << "Server Name" << std::endl;
			parseServerName(iss, server);
			break ;
		case root:
			std::cout << "Parse root" << std::endl;
			parseRoot(iss, server);
			break ;
		case location:
			std::cout << "Parse location" << std::endl;
			parseLocation(iss, server);
			break ;
		default:
			std::cout << "Next word" << std::endl;
		}	
 
    } while (end == 0);
	this->_ports.push_back(*server);
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
	try
	{
		Config S(argv[1]);
		std::cout << std::endl << std::endl << std::endl;
		
		std::vector<ServerConfig> ports = S.get_servers();
		int ports_size = ports.size();
		std::vector<ServerConfig>::iterator iter;
		for(iter = ports.begin(); iter < ports.end(); iter++)
		{
			std::cout << iter->name << std::endl;
			std::cout << (*iter).listen << std::endl;
			std::cout << (*iter).root << std::endl;
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	catch (std::invalid_argument& e)
	{
        std::cerr << "Config error:" << e.what() << std::endl;
        return -1;
    }
}