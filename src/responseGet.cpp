/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:08:23 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/11 19:36:13 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseGet.hpp"

/*============================================================================*/
ResponseGet::ResponseGet( void ) : ResponseBase() { }

ResponseGet::ResponseGet( std::string filePath ) : ResponseBase() {
	this->_path.append(filePath);
	checkPath();
	generateResponse();
}

ResponseGet::~ResponseGet( void ) { }
/*============================================================================*/

static std::string fileExtension( const std::string& filename ) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos != std::string::npos && dotPos < filename.length() - 1) {
		std::string foundExtension = filename.substr(dotPos + 1);
		return (foundExtension);
	}
	return "INVALID";
}


void ResponseGet::checkPath( void ) {
	this->_isImg = false;

	if (this->_path.empty()) {
		setContentType("html");
		this->_path.append("html/index.html");
	}
	else {
		setContentType(fileExtension(this->_path));
		if (this->_contentType == "png" || this->_contentType == "jpg" || this->_contentType == "jpeg" || this->_contentType == "ico")
			this->_isImg = true;
	}

	setStatusCodeGet();
}

void ResponseGet::setStatusCodeGet( void ) {
	this->_file.open(this->_path);

	if (this->_file.is_open() && (this->_contentTypes.find(this->_contentType) != this->_contentTypes.end()))
		setStatusCode(200);
	else {
		setStatusCode(404);
		_file.open("html/" + std::to_string(_statusCode) + ".html");
	}
}


void ResponseGet::generateResponse( void ) {
	this->_response.clear();

	std::string type = this->_contentTypes[this->_contentType];
	std::cout << "Content type: " << _contentType << std::endl;
	try {
		this->_response.append("HTTP/1.1 " + std::to_string(this->_statusCode) + " " + this->_statusCodes[this->_statusCode] + "\r\n");

		if (this->_isImg) {
			std::cout << "Content-type: " << type << std::endl;
			this->_response.append("Content-Type: " + type + "\r\n\r\n");

			char img_buffer[1024];
			while (!this->_file.eof()) {
				this->_file.read(img_buffer, sizeof(img_buffer));
				this->_response.append(img_buffer, this->_file.gcount());
			}
		}
		else {
			this->_response.append("Content-Type: " + type + "\r\n\r\n");
			std::string line;
			while (std::getline(this->_file, line))
				this->_response.append(line);
		}

		// if (_file.fail()) { throw std::runtime_error("Error"); }
		this->_file.close();
	}
	catch (std::exception &e) {
		this->_response.clear();
		
		this->_response.append(ISE_500);
		std::string body = std::string(ISE_HTML);
		this->_contentLength = body.length();
		this->_response.append(std::to_string(this->_contentLength));
		this->_response.append(ISE_HTML);
	}
}


std::string ResponseGet::getResponse( void ) const {
	return (this->_response);
}
