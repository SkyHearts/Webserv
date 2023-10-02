/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseDelete.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:54:10 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/02 16:59:29 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseDelete.hpp"

/*============================================================================*/

ResponseDelete::ResponseDelete( void ) : ResponseBase() { }

ResponseDelete::ResponseDelete( std::string filePath, ServerConfig portinfo ) : ResponseBase() {
	this->_portinfo = portinfo;
	this->_path.append(filePath);

	if (validateResource(this->_path)) {
		if (checkPermissions("DELETE"))
			deleteData();
		else setStatusCodeDelete(405);
	}
	else setStatusCodeDelete(404);

	generateResponse();
}

ResponseDelete::ResponseDelete( void ) { }
/*============================================================================*/

// void ResponseDelete::clearResources( void ) {}

bool ResponseDelete::validateResource( const std::string &name ) {
	struct stat sb;

	if (stat(name.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
		return true;
	return false;
}

void ResponseDelete::setStatusCodeDelete( int status ) {
	setStatusCode(status);
	setContentType("html");
}

void ResponseDelete::generateResponse( void ) {
	if (this->_statusCode == 500)
		this->_response.append(generateResponseISE());
	else {
		this->_response.append("HTTP/1.1 " + std::to_string(this->_statusCode) + " " + this->_statusCodes[this->_statusCode] + "\r\n");
		this->_response.append("Content-Type: " + this->_contentTypes["html"] + "\r\n\r\n");
		std::string line;
		while (getline(this->_file, line))
			this->_response.append(line);
	}
}