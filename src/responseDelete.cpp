/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseDelete.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:54:10 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/02 19:00:12 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseDelete.hpp"

/*============================================================================*/

ResponseDelete::ResponseDelete( void ) : ResponseBase() { }

ResponseDelete::ResponseDelete( std::string filePath, ServerConfig portinfo ) : ResponseBase() {
	this->_portinfo = portinfo;
	this->_filePath.append(this->_portinfo.root + filePath);

	if (validateResource(this->_filePath)) {
		this->_path.append(this->_filePath.substr(0, this->_filePath.find_last_of('/')));
		if (checkPermissions("DELETE")) //fuck this needs directory
			deleteData();
		else setStatusCodeDelete(405, 0);
	}
	else setStatusCodeDelete(404, 0);

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

void ResponseDelete::setStatusCodeDelete( int status, bool isUpload ) {
	setStatusCode(status);
	setContentType("html");

	this->_path.append(this->_portinfo.root + "/");
	if (isUpload)
		this->_path.append("uploads/");
	this->_path.append(std::to_string(this->_statusCode) + ".html");

	this->_file.open(this->_path);
	if (!this->_file.is_open())
		setStatusCode(500);
}

void ResponseDelete::deleteData( void ) {
	if (remove(this->_filePath.c_str()) == 0)
		setStatusCodeDelete(200, 1);
	else
		setStatusCode(500);
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