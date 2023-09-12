/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseBase.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:38:01 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/12 15:04:47 by nnorazma         ###   ########.fr       */
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
	_statusCodes[404] = "Not Found";
	_statusCodes[405] = "Method Not Allowed";
	_statusCodes[413] = "Payload Too Large";
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

std::string ResponseBase::getResponse( void ) const {
	return (_response);
}
