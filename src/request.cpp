/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/27 17:14:57 by jyim             ###   ########.fr       */
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

size_t Request::getPayload( void ) const {
	return (_payloadSize);
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
std::string Request::processRequest( std::string req, int req_len, ServerConfig portinfo ) {
	clearResources();

	_request = req;
	_payloadSize = req_len;
	parseRequest();

	if (_method == "GET") {
		ResponseGet get(_path, portinfo);
		_response = get.getResponse();
	}
	else if (_method == "POST") {
		ResponsePost post(this->_path, getHeader(), getBody(), getPayload(), portinfo);
		// for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); ++it)
		// 	std::cout << it->first << " => " << it->second << '\n';
		// std::cout << getBody() << std::endl;
		_response = post.getResponse();
		
	}
	// else if (_method == "DELETE") {
	// 	responseDelete del;
	// 	_response = del.getResponse(this->_path, getHeader(), getBody(), getPayload());
	// }

	else {
		ResponseGet unknown(portinfo.errorPages[501], portinfo);
		_response = unknown.getResponse();
	}
	
	//if path directory is cgi-bin and valid cgi program/file
	// cgi_handler cgi;

	// cgi.execCGI(this->_content, "html/" + this->_path, );
	// 	_response = unknown.getResponse();

	return _response;
}