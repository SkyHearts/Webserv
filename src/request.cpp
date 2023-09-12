/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/12 18:54:31 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

/*============================================================================*/

Request::Request( void ) {}

Request::~Request( void ) {}

/*============================================================================*/

void Request::parseRequest( void ) {
	_request.erase(remove(_request.begin(), _request.end(), '\r'), _request.end()); //line break in request is \r\n, this removes \r

	std::stringstream request(_request);
	std::string line, key, value;

	getline(request, line);
	std::istringstream head(line);
	head >> _method >> _path >> _http;

	//Store rest of header in a map.
	_path.erase(0, 1);

	int headsize = 0;
	while (getline(request, line, '\n') && !line.empty()) {
		if (line == "\r")
			break;
		if (line == "\r\n\r\n")
			break;
		try {
			key = line.substr(0, line.find(':'));
			value = line.substr(line.find(':') + 2);
			std::cout.flush();
		}
		catch (std::exception const &e) {
			break ;
		} 
		_content.insert(std::pair< std::string, std::string >(key, value));
		headsize++;
	}

	//Store put content.
	std::cout << "\nheadsize: " << headsize << std::endl;
	if (!request.eof()) {
		char temp[1024];
		while (!request.eof()) {
			request.read(temp, sizeof(temp));
			_contentBody.append(temp);
		}
		std::cout << "contentbody:\n[" << _contentBody << "]" << std::endl;
	}
	
}

std::string Request::processRequest( std::string req ) {
	_request = req;
	parseRequest();
	
	if (_method == "GET") {
		ResponseGet get(_path);
		_response = get.getResponse();
	}
	// else if (_method == "POST") {
	// 	responsePost post;
	// 	_response = post.getResponse();
	// }
	// else if (_method == "DELETE") {
	// 	responseDelete del;
	// 	_response = del.getResponse();
	// }
	else {
		ResponseUnknown unknown;
		_response = unknown.getResponse();
	}

	return _response;
}