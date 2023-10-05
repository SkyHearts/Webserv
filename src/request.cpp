/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m4rrs <m4rrs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/04 22:57:22 by m4rrs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "cgi_handler.hpp"

/*============================================================================*/

Request::Request( void ) {}

Request::~Request( void ) {}

/*============================================================================*/

/*
	Clear all resource buffers
*/
void Request::clearResources( void ) {
	this->_request.clear();
	this->_method.clear();
	this->_path.clear();
	this->_http.clear();
	this->_response.clear();
	this->_header.clear();
	this->_body.clear();
}

std::map< std::string, std::string > Request::getHeader( void ) const {
	return (_header);
}

std::string Request::getBody( void ) const {
	return (_body);
}

/*
	Parse and save a client request into its
	header and content components
*/
void Request::parseRequest() {
	std::istringstream requestStream(_request);
	std::string line;
	std::string key, value;

	if (std::getline(requestStream, line)) {
		std::istringstream head(line);
		head >> _method >> _path >> _http;
	}
	_header.insert(std::pair< std::string, std::string >("Method", _method));
	_header.insert(std::pair< std::string, std::string >("Path", _path));

	while (std::getline(requestStream, line, '\n') && !line.empty()) {
		if (line == "\r")
			break ;
		if (line == "\r\n\r\n")
			break ;
		try {
			key = line.substr(0, line.find(':'));
			value = line.substr(line.find(':') + 2);
			std::cout.flush();
		}
		catch (std::exception const &e) {
			break ;
		} 
		_header.insert(std::pair< std::string, std::string >(key, value));
	}

	std::ostringstream bodyStream;
	while (std::getline(requestStream, line, '\n'))
		bodyStream << line << "\n";
	_body = bodyStream.str();
}

/*
	Request handler to generate responses based on
	the type of client request
*/
std::string Request::processRequest( std::string req, ServerConfig portinfo ) {
	clearResources();

	_request = req;
	parseRequest();

	if (_method == "GET") {
		ResponseGet get(_path, portinfo);
		_response = get.getResponse();
	}
	else if (_method == "POST") {
		ResponsePost post(this->_path, getHeader(), getBody(), portinfo);
		_response = post.getResponse();
		
	}
	else if (_method == "DELETE") {
		ResponseDelete del(this->_path, portinfo);
		_response = del.getResponse();
	}
	else {
		ResponseGet unknown("unknown", portinfo);
		_response = unknown.getResponse();
	}

	return _response;
}