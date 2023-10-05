/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m4rrs <m4rrs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:37:14 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/04 23:48:01 by m4rrs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"
#include "responseGet.hpp"
#include "responsePost.hpp"
#include "responseDelete.hpp"

class Request {
	private:
		std::string _request, _response, _method, _path, _http, _body;
		std::map< std::string, std::string > _header;
		// std::string _response;
		// std::string _method;
		// std::string _path;
		// std::string _http;
		// std::string _body;

	public:
		Request( void );
		~Request( void );

		void clearResources( void );
		void parseRequest( void );
		std::string processRequest( std::string req, ServerConfig portinfo );
		std::string getBody( void ) const;
		std::map< std::string, std::string > getHeader( void ) const;
};