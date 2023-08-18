/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:01:43 by jyim              #+#    #+#             */
/*   Updated: 2023/08/18 18:22:42 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse.hpp"
#include <string>
#include <fstream>
#include <sstream>

Config::Config(){
	//USE DEFAULT CONF FILE if there are no files specified
}

Config::~Config( void ){
	std::cout << "Deconstruct Config" << std::endl;
}

std::string	Config::getstring(std::fstream &file){

	
}

void	Config::parse(std::fstream &file)
{
	std::string str = config.getstring(file); // write function to write whole file? or change to parse perline
	std::istringstream iss(str);
	do {
    	std::string subs;
        // Get the word from the istringstream
        iss >> subs;
 
        // Print the word fetched
        // from the istringstream
        std::cout << subs << std::endl;

		// parse to correct variable
 
    } while (iss);
}

Config::Config(char *av)
{
	//std::string serverConfig = av;
	std::fstream file;
	file.open(av, std::ios::in);
	if (!file)
		std::cout << "File not found" << std::endl;
	Config::parse(file);
}
