/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:37:14 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/13 16:03:35 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"
#include "responseGet.hpp"
#include "responsePost.hpp"
#include "responseDelete.hpp"
#include "responseUnknown.hpp"

class Request {
	private:
		std::string _request;
		std::string _response;
		std::string _method;
		std::string _path;
		std::string _http;
		std::string _body;
		std::map< std::string, std::string > _header;


	public:
		Request( void );
		~Request( void );

		void clearResources( void );
		void parseRequest( void );
		std::string processRequest( std::string req );
		std::map< std::string, std::string > getHeader( void ) const;
		std::string getBody( void ) const;
};