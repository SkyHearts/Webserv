/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 00:34:00 by jyim              #+#    #+#             */
/*   Updated: 2023/09/27 00:34:00 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/autoindex.hpp"
#include "../includes/parse.hpp"
// #include <dirent.h>

// if URI ends with / and location autoindex = True, run here
autoindex::~autoindex(){

}

std::string autoindex::getContentValue(std::string key){
	return(this->(_content[key]));
}

std::string autoindex::generateList(ServerConfig portinfo){
	this->configinfo = portinfo;
	std::string currentdir = configinfo.root + getContentValue("Path");
	if (!isValidDir(currentdir))
		return "Directory not found";
	DIR* dir = opendir(currentdir);
	if (!dir)
		return "Directory not found";
	struct stat info;
	struct dirent* item;
	while (item = readdir(dir) != NULL){
		if (stat(path, &info) == 0){
			
		}
	}
}


bool isValidDir( const char *path ) {
	struct stat info;

	// std::cout << "isvaliddir path = " << path << std::endl;
	if (stat(path, &info) == 0) {
		// if (S_ISDIR(info.st_mode))
		// 	std::cout << path << " is a directory" << std::endl;
		// else 
		// 	std::cout << path << " is not a directory" << std::endl;
		return S_ISDIR(info.st_mode);
	}
	else
		return 0;
}

/*
	If path is regular file return true, /test/test.html = true
	Else return false, /test/ = false
*/
bool isValidFile( const char *path ) {
	struct stat info;

	// std::cout << "isvalidfile path = " << path << std::endl;
	if (stat(path, &info) == 0) {
		// if (S_ISREG(info.st_mode))
		// 	std::cout << path << " is a regular file" << std::endl;
		// else 
			// std::cout << path << " is not a regular file" << std::endl;

		return S_ISREG(info.st_mode);
	}
	else
		return 0;
}