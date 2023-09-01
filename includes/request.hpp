/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:37:14 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/01 17:49:21 by nnorazma         ###   ########.fr       */
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
		std::string _method;
		std::string _path;
		std::string _http;
		std::map< std::string, std::string > _content;

	public:
		Request( void );
		~Request( void );
		void parseRequest( void );
		std::string Request::processRequest( std::string req );
};