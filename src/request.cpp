/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/05 15:31:21 by nnorazma         ###   ########.fr       */
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
	return (this->_header);
}

std::string Request::getBody( void ) const {
	return (this->_body);
}

/*
	Parse and save a client request into its
	header and content components
*/
void Request::parseRequest() {
	std::istringstream requestStream(this->_request);
	std::string line;
	std::string key, value;

	if (std::getline(requestStream, line)) {
		std::istringstream head(line);
		head >> this->_method >> this->_path >> this->_http;
	}
	_header.insert(std::pair< std::string, std::string >("Method", this->_method));
	_header.insert(std::pair< std::string, std::string >("Path", this->_path));

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
		this->_header.insert(std::pair< std::string, std::string >(key, value));
	}

	std::ostringstream bodyStream;
	while (std::getline(requestStream, line, '\n'))
		bodyStream << line << "\n";
	this->_body = bodyStream.str();
}

/*
	Request handler to generate responses based on
	the type of client request
*/
std::string Request::processRequest( std::string req, ServerConfig portinfo ) {
	clearResources();

	this->_request = req;
	parseRequest();

	if (this->_method == "GET") {
		ResponseGet get(this->_path, portinfo);
		this->_response = get.getResponse();
	}
	else if (this->_method == "POST") {
		ResponsePost post(this->_path, getHeader(), getBody(), portinfo);
		this->_response = post.getResponse();
		
	}
	else if (this->_method == "DELETE") {
		ResponseDelete del(this->_path, portinfo);
		this->_response = del.getResponse();
	}
	else {
		ResponseGet unknown("unknown", portinfo);
		this->_response = unknown.getResponse();
	}

	return this->_response;
}