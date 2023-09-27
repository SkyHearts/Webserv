/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:53:55 by jyim              #+#    #+#             */
/*   Updated: 2023/09/23 18:09:17 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "headers.hpp"

//typedef struct env
//{
//	std::string SERVER_SOFTWARE;
//	int 		SERVER_PORT;
//	std::string REQUEST_METHOD;
//	std::string PATH_INFO;
//	std::string PATH_TRANSLATED;
//	std::string HTTP_REFERER;
//	std::string HTTP_ACCEPT;
//} WebEnv;

//To help return response?
// class uri
// {
// 	private:
// 	void parsehttp(std::string url);
// 	public:
// 	//Get func
// 	std::string path, name, host;
// 	int port;
// };

class cgi_handler
{
	private:
	//WebEnv _structEnv;
	char **_env;
	char **_arg;
	public:
	cgi_handler();
	~cgi_handler();
	
	//Get funct _env;
	char** getEnv(void) { return _env; }

	//Utils
	void reassginDArray(char **dest, char **src);
	void delDArray(char **dArray);


	//Set _env
	void createEnv(std::map<std::string, std::string> content, char** payload, ServerConfig portInfo);
	void addCharEnvs(char** payload);
	void addEnv(std::string envVar);
	
	//Set argv, arg[0] = path to cgi
	void addArg(std::string arg);
	void createArg(std::string path);
	
	//Execute in child process using execve
	void execCGI(std::map<std::string, std::string> content, std::string path, ServerConfig portInfo, char** payload = NULL);
};

int getCharDArraySize(char** array);