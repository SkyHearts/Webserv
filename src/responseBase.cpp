/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseBase.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:38:01 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/03 15:20:21 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseBase.hpp"

/*============================================================================*/
ResponseBase::ResponseBase( void ) {
	initStatusCodes();
	initContentTypes();
}

ResponseBase::~ResponseBase( void ) { }
/*============================================================================*/

void ResponseBase::initStatusCodes( void ) {
	_statusCodes[200] = "OK";
	_statusCodes[201] = "Created";
	_statusCodes[202] = "Accepted";
	_statusCodes[400] = "Bad Request";
	_statusCodes[404] = "Not Found";
	_statusCodes[405] = "Method Not Allowed";
	_statusCodes[409] = "Conflict";
	_statusCodes[413] = "Payload Too Large";
	_statusCodes[500] = "Internal Server Error";
	_statusCodes[501] = "Not Implemented";
}

void ResponseBase::initContentTypes( void ) {
	_contentTypes["html"] = "text/html";
	_contentTypes["css"] = "text/css";
	_contentTypes["js"] = "text/javascript";
	_contentTypes["png"] = "image/png";
	_contentTypes["jpg"] = "image/jpeg";
	_contentTypes["jpeg"] = "image/jpeg";
	_contentTypes["ico"] = "image/x-icon";
	_contentTypes["plain"] = "text/plain";
	_contentTypes["form"] = "multipart/form-data";
}

void ResponseBase::setContentType ( std::string type ) {
	_contentType = type;
}

void ResponseBase::setStatusCode ( int code ) {
	_statusCode = code;
}

std::map< std::string, std::string > ResponseBase::getContentTypes( void ) const {
	return (_contentTypes);
}

std::map< int, std::string > ResponseBase::getStatusCodes( void ) const {
	return (_statusCodes);
}

std::string ResponseBase::getResponse( void ) {
	return (_response);
}

bool ResponseBase::checkPermissions( std::string method ) {
	bool found = false;

	if (this->_path.find("assets/") == 0)
		found = true;

	if (_path == _portinfo.root + '/' + _portinfo.index)
		found = true;

	for (std::vector<Location>::iterator it = this->_portinfo.locations.begin(); it != this->_portinfo.locations.end(); it++) {
		if (this->_path.find((*it).uri) != std::string::npos) {
			found = false;
			for (std::vector<std::string>::iterator it2 = (*it).allowedMethods.begin(); it2 != (*it).allowedMethods.end(); it2++) {
				if (*it2 == method)
					found = true;
			}
		}
	}
	return (found);
}

std::string ResponseBase::generateResponseISE ( void ) {
	std::string response;

	response.append(ISE_500);
	std::string body = ISE_MESSAGE;
	this->_contentLength = body.length();
	response.append(std::to_string(this->_contentLength));
	response.append(body);

	return (response);
}

std::string decodeEncoding( std::string &input ) {
	std::string decoded;
	size_t input_len = input.length();
	size_t pos = 0;
	int ascii;

	while (pos < input_len) {
		if (input[pos] == '%' && (pos + 2) < input_len) {
			char hex[3] = { input[pos + 1], input[pos + 2], 0 };

			if (sscanf(hex, "%x", &ascii) == 1) {
				decoded += static_cast<char>(ascii);
				pos += 3;
			}
			else
				decoded += input[pos++];
		}
		else
			decoded += input[pos++];
	}

	return decoded;
}