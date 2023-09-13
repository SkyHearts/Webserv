/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUnknown.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:47:38 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/12 17:26:09 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseUnknown.hpp"

/*============================================================================*/

ResponseUnknown::ResponseUnknown( void ) {
	setStatusCode(501);
	setContentType("plain");
	generateResponse();
}

ResponseUnknown::~ResponseUnknown( void ) {}

/*============================================================================*/

void ResponseUnknown::generateResponse( void ) {
	this->_response.clear();

	this->_response.append("HTTP/1.1 " + std::to_string(this->_statusCode) + " " + this->_statusCodes[_statusCode] + "\r\n");
	this->_response.append("Content-Type: " + this->_contentTypes[_contentType] + "\r\n\r\n");
	this->_response.append("Method requested was not implemented.");

	std::cout << RED << "RESPONSE:\n" << _response << CLEAR << std::endl;
}