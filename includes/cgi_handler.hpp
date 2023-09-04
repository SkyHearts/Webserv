/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:53:55 by jyim              #+#    #+#             */
/*   Updated: 2023/09/04 17:18:20 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "headers.hpp"

typedef struct env
{
	std::string SERVER_SOFTWARE;
	int 		SERVER_PORT;
	std::string REQUEST_METHOD;
	std::string PATH_INFO;
	std::string PATH_TRANSLATED;
	std::string HTTP_REFERER;
	std::string HTTP_ACCEPT;
} WebEnv;

class cgi_handler
{
	private:
	WebEnv _structEnv;
	char **_structEnv_env;
	public:
	cgi_handler();
	~cgi_handler();

	//Get funct;
	int getServerPort(void) {return _structEnv.SERVER_PORT;}
	std::string getServerSoftware(void) {return _structEnv.SERVER_SOFTWARE;}
	std::string getRequestMethod(void) {return _structEnv.REQUEST_METHOD;}
	std::string getPathInfo(void) {return _structEnv.PATH_INFO;}
	std::string getPathTranslated(void) {return _structEnv.PATH_TRANSLATED;}
	std::string getHTTPReferer(void) {return _structEnv.HTTP_REFERER;}
	std::string getHTTPAccept(void) {return _structEnv.HTTP_ACCEPT;}

	void create_env(void);
	void setEnv(char** env);
	void exec_cgi(WebEnv env);
};