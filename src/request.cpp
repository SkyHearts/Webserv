/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/08/29 14:59:17 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

/*============================================================================*/

Request::Request( void ) {}
Request::~Request( void ) {}

/*============================================================================*/

void Request::parseRequest( std::string req ) {

	req.erase(remove(req.begin(), req.end(), '\r'), req.end()); //line break in request is \r\n, this removes \r

	std::stringstream request(req);
	std::string line, key, value;

	getline(request, line);
	std::istringstream head(line);
	head >> _method >> _path >> _http;

	_headSize = 0;
	while (getline(request, line, '\n')) {
		try {
			key = line.substr(0, line.find(':'));
			value = line.substr(line.find(':') + 2);
			std::cout.flush();
		}
		catch (std::exception const &e) {
			break ;
		} 
		_content.insert(std::pair< std::string, std::string >(key, value));
		_headSize++;
	}
	// how do i handle put content ???
}