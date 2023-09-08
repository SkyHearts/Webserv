/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:53:55 by jyim              #+#    #+#             */
/*   Updated: 2023/09/07 13:15:42 by jyim             ###   ########.fr       */
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
class cgi_response
{
	protected:
	std::string _header;
	public:
	//Get func
	std::string getHeader(void) {return _header;}
};

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

	//Set _env
	void createEnv(void);
	void addEnv(std::string envVar);
	void delEnv(char **env);
	void reassginEnv(char **dest, char **src);
	
	//Set argv, arg[0] = path to cgi
	void createArg(void);
	
	//Execute in child process using execve
	void execCGI(std::string path, char **argv);
};

int getCharDArraySize(char** array);