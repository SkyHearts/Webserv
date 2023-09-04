/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:37:14 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/04 15:58:09 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"

enum request {
	GET,
	POST,
	DELETE,
	UNKNOWN
};


class Request {
	private:
		std::string _request;
		std::string _response;

		int	_headSize;
		// size_t _payloadSize;
		std::string _method;
		std::string _path;
		std::string _http;

		std::ifstream _file;
		int _statusCode;

		std::map< std::string, std::string > _content;
		std::map< int, std::string > _statusCodes;

	public:
		Request( void );
		~Request( void );

		void initStatusCodes( void );
		void parseRequest( void );
		void setResponse( void );
		void checkPath( void );

		std::string processRequest( std::string req );
};