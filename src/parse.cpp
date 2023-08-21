/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:01:43 by jyim              #+#    #+#             */
/*   Updated: 2023/08/21 17:21:12 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse.hpp"
#include <string>
#include <fstream>
#include <sstream>

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

void	Config::parseServerBlock(std::istringstream &iss){
	std::map<std::string, serverBlock> s_mapStringValues;
	Config::initEnum(s_mapStringValues);
	ServerConfig server;
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
			//server.listen = ;
			//parseListen(iss);
			std::cout << "Parse Listen" << std::endl;
			break ;
		case server_name:
			//parseServerName(iss);
			std::cout << "Server Name" << std::endl;
			break ;
		default:
			std::cout << "Wrong string" << std::endl;
		}	
 
    } while (end == 0);
		//this->_ports.push_back(server);
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
	std::string serverConfig = Config::getstring(av);
	Config::parse(serverConfig);
}


int main(int argc, char **argv){
	Config S(argv[1]);
	//std::vector<ServerConfig> ports = Server.get_servers();
	//int ports_size = ports.size();

	//std::vector<ServerConfig>::iterator iter = ports.begin();	
	//for(iter; iter < ports.end(); iter++)
	//{
    //	std::cout << (*iter).name << std::endl;
	//}
	std::cout << std::endl;
}