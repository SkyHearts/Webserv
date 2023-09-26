/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/27 01:57:44 by jyim             ###   ########.fr       */
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
	this->_response.clear();
	this->_method.clear();
	this->_path.clear();
	this->_http.clear();
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
void Request::parseRequest( void ) {
	_request.erase(remove(_request.begin(), _request.end(), '\r'), _request.end()); //line break in request is \r\n, this removes \r

	std::stringstream request(_request);
	std::string line, key, value;

	getline(request, line);
	std::istringstream head(line);
	head >> _method >> _path >> _http;
    _header.insert(std::pair< std::string, std::string >("Method", _method));
    _header.insert(std::pair< std::string, std::string >("Path", _path));

	while (getline(request, line, '\n') && !line.empty()) {
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

	if (!request.eof()) {
		char temp[1024];
		std::memset(temp, 0, sizeof(temp));
		while (!request.eof()) {
			request.read(temp, sizeof(temp));
			_body.append(temp);
		}
		if (_body.size() > 0)
			std::cout << "Body:\n[" << _body << "]" << std::endl;
	}
}

/*
	Request handler to generate responses based on
	the type of client request
*/
std::string Request::processRequest( std::string req, ServerConfig portinfo ) {
	clearResources();

	_request = req;
	_payloadSize = req.size();
	parseRequest();

	if (_method == "GET") {
		ResponseGet get(_path, portinfo);
		_response = get.getResponse();
	}
	// else if (_method == "POST") {
	// 	ResponsePost post(this->_path, getHeader(), getBody(), getPayload());
	// 	_response = post.getResponse();
	//	
	// }
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