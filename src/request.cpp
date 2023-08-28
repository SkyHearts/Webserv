/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/08/28 17:03:49 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

/*============================================================================*/

Request::Request( void ) {}
Request::~Request( void ) {}

/*============================================================================*/

void Request::parseRequest( std::string req ) {

	std::cout << BLUE << "before split: " << req << CLEAR << std::endl;
	req.erase(remove(req.begin(), req.end(), '\r'), req.end());
	std::cout << YELLOW << "split: " << req << CLEAR << std::endl;

	std::stringstream request(req);
	std::string line, key, value;

	getline(request, line);
	std::istringstream head(line);
	head >> _method >> _path >> _http;

	std::cout << "head [" << _method << "] [" << _path << "] [" << _http << "] " << std::endl;

	int i = 0;
	while (getline(request, line, '\n')) {
		// _content["method"] = "GET";
		try {
			std::cout << RED << line << "|" << CLEAR << std::endl;
			key = line.substr(0, line.find(':'));
			std::cout << "key = " << key << " || ";
			value = line.substr(line.find(':') + 2);
			std::cout  << "value = " << value << "<END>" << std::endl;

			std::cout << GREEN << i << " ->\t";
			std::cout << "key = " << key << " || value = " << value << "<END>" << CLEAR << std::endl;
			std::cout.flush();
		}
		catch (std::exception const &e) {
			std::cerr << "Error retrieving value." << std::endl;
			break ;
		} 

		_content.insert(std::pair< std::string, std::string >(key, value));
		i++;
		// std::cout << "key = " << " || value = " << value << std::endl;
	}
}