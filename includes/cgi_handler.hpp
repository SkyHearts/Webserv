/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:53:55 by jyim              #+#    #+#             */
/*   Updated: 2023/10/05 15:19:17 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "headers.hpp"

class cgi_handler
{
	private:
		//WebEnv _structEnv;
		char **_env;
		char **_arg;

	public:
		cgi_handler( void );
		~cgi_handler( void );
		
		//Get funct _env;
		char** getEnv( void ) { return _env; }

		//Utils
		void reassginDArray( char **dest, char **src );
		void delDArray( char **dArray );

		//Set _env
		void createEnv( std::map< std::string, std::string > content, char** payload, ServerConfig portInfo );
		void addCharEnvs( char** payload );
		void addEnv( std::string envVar );
		
		//Set argv, arg[0] = path to cgi
		void addArg( std::string arg );
		void createArg( std::string path );
		
		//Execute in child process using execve
		std::string execCGI( std::map< std::string, std::string > content, std::string path, ServerConfig portInfo, char** payload = NULL );
};

int getCharDArraySize( char** array );