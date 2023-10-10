/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:00:42 by jyim              #+#    #+#             */
/*   Updated: 2023/10/05 15:19:38 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "headers.hpp"
#include "server.hpp"

class Config {
	private:
		std::vector<ServerConfig>	_ports;
		Config( void );

	public:
		Config( std::string config_file );
		~Config( void );

		bool	CheckPortTCP( short int dwPort, const char *ipAddressStr );
		void	initEnumServerBlock( std::map<std::string, serverBlock> &s_mapStringValues );
		void	parse( std::string &file );
		void	parseServerBlock( std::istringstream &iss );
		void	parseListen( std::istringstream &iss, ServerConfig *server );
		void	parseServerName( std::istringstream &iss, ServerConfig *server );
		void	parseRoot( std::istringstream &iss, ServerConfig *server );
		void	parseIndexServ( std::istringstream &iss, ServerConfig *server );
		void	parseLocation( std::istringstream &iss, ServerConfig *server );
		void	parseLocationParams( std::istringstream &iss, ServerConfig *server, struct Location *loc, std::string& update );
		void	parseErrorPages( std::istringstream &iss, ServerConfig *server );
		void	parseClientBodySize( std::istringstream &iss, ServerConfig *server );
		
		std::vector<ServerConfig> get_servers( void ) { return _ports; }
		std::string	getstring( std::string config_file );

		void	checkDupPorts( void );
        void	checkDefaults( ServerConfig &server );
};

bool	is_punct( int c );
bool	checkAlpha( const std::string &str );
bool	checkNum( const std::string &str );
bool	containsDuplicate( const std::vector<int>& nums );