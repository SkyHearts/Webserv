/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseBase.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:38:01 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/05 18:59:32 by nnorazma         ###   ########.fr       */
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
	this->_statusCodes[200] = "OK";
	this->_statusCodes[201] = "Created";
	this->_statusCodes[202] = "Accepted";
	this->_statusCodes[308] = "Permanent Redirect";
	this->_statusCodes[400] = "Bad Request";
	this->_statusCodes[404] = "Not Found";
	this->_statusCodes[405] = "Method Not Allowed";
	this->_statusCodes[409] = "Conflict";
	this->_statusCodes[413] = "Payload Too Large";
	this->_statusCodes[500] = "Internal Server Error";
	this->_statusCodes[501] = "Not Implemented";
}

void ResponseBase::initContentTypes( void ) {
	this->_contentTypes["html"] = "text/html";
	this->_contentTypes["css"] = "text/css";
	this->_contentTypes["js"] = "text/javascript";
	this->_contentTypes["png"] = "image/png";
	this->_contentTypes["jpg"] = "image/jpeg";
	this->_contentTypes["jpeg"] = "image/jpeg";
	this->_contentTypes["ico"] = "image/x-icon";
	this->_contentTypes["plain"] = "text/plain";
	this->_contentTypes["form"] = "multipart/form-data";
}

void ResponseBase::setContentType ( std::string type ) {
	this->_contentType = type;
}

void ResponseBase::setStatusCode ( int code ) {
	this->_statusCode = code;
}

std::map< std::string, std::string > ResponseBase::getContentTypes( void ) const {
	return (this->_contentTypes);
}

std::map< int, std::string > ResponseBase::getStatusCodes( void ) const {
	return (this->_statusCodes);
}

std::string ResponseBase::getResponse( void ) {
	return (this->_response);
}

bool ResponseBase::checkPermissions(std::string method) {
	if (this->_path.find("assets/") == 0)
		return true;

	if (this->_path == this->_portinfo.root + '/' + this->_portinfo.index)
		return true;

	for (std::vector<Location>::iterator it = this->_portinfo.locations.begin(); it != this->_portinfo.locations.end(); it++) {
		if (this->_path.find((*it).uri) != std::string::npos) {
			for (std::vector<std::string>::iterator it2 = (*it).allowedMethods.begin(); it2 != (*it).allowedMethods.end(); it2++) {
				if (*it2 == method) {
					return true;
				}
			}
		}
	}
	return false;
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