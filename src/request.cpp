/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/18 16:11:21 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "cgi_handler.hpp"

/*============================================================================*/

Request::Request( void ) {
	_request = new char[65535];
	std::memset(_request, 0, 65535);
}

Request::~Request( void ) {
	if (_request != nullptr) {
		free(_request);
		_request = nullptr;
	}
}

/*============================================================================*/

/*
	Clear all resource buffers
*/
void Request::clearResources( void ) {
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
	char* line = strtok(_request, "\r\n");
	std::string key, value;

	if (line != nullptr) {
		std::istringstream head(line);
		head >> _method >> _path >> _http;
	}

	while ((line = strtok(nullptr, "\r\n")) != nullptr && line[0] != '\0') {
		if (strcmp(line, "\r") == 0)
			break;

		try {
			char* colon_pos = strchr(line, ':');
			if (colon_pos != nullptr) {
				*colon_pos = '\0';
				key = line;
				value = colon_pos + 2;
				_header.insert(std::pair<std::string, std::string>(key, value));
			}
		}
		catch (std::exception const &e) {
			break;
		}	
	}

	char temp[1024];
	std::memset(temp, 0, sizeof(temp));
	while ((line = strtok(nullptr, "\r\n")) != nullptr) {
		strcat(temp, line);
	}
	if (strlen(temp) > 0) {
		_body = temp;
		std::cout << "Body:\n[" << _body << "]" << std::endl;
	}
}

/*
	Request handler to generate responses based on
	the type of client request
*/
std::string Request::processRequest( char *req, ServerConfig portinfo ) {
	clearResources();

	_request = req;
	_payloadSize = strlen(req);
	parseRequest();

	if (_method == "GET") {
		ResponseGet get(_path, portinfo);
		_response = get.getResponse();
	}
	// else if (_method == "POST") {
	// 	ResponsePost post(this->_path, getHeader(), getBody(), getPayload());
	// 	_response = post.getResponse();
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