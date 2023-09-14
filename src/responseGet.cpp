/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:08:23 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/14 19:25:34 by nnorazma         ###   ########.fr       */
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

	_path.erase(0, 1);
	if (this->_path.empty()) {
		setContentType("html");
		this->_path.append("html/index.html");
	}
	else {
		setContentType(fileExtension(this->_path));
		if (this->_contentType == "png" || this->_contentType == "jpg" || this->_contentType == "jpeg" || this->_contentType == "ico")
			this->_isImg = true;
		if (this->_contentType == "html")
			_path.insert(0, "html/");
	}

	setStatusCodeGet();
}

void ResponseGet::setStatusCodeGet( void ) {
	this->_file.open(this->_path);

	if (this->_file.is_open() && (this->_contentTypes.find(this->_contentType) != this->_contentTypes.end())) {
		if (_path == "html/501.html") { setStatusCode(501); }
		else { setStatusCode(501); }
	}
	else {
		setStatusCode(404);
		setContentType("html");
		_file.open("html/" + std::to_string(_statusCode) + ".html");
	}
}


void ResponseGet::generateResponse( void ) {
	this->_response.clear();

	try {
		this->_response.append("HTTP/1.1 " + std::to_string(this->_statusCode) + " " + this->_statusCodes[this->_statusCode] + "\r\n");

		if (this->_isImg) {
			this->_response.append("Content-Type: " + this->_contentTypes[this->_contentType] + "\r\n\r\n");

			char imgBuffer[1024];
			std::memset(imgBuffer, 0, sizeof(imgBuffer));
			while (!this->_file.eof()) {
				this->_file.read(imgBuffer, sizeof(imgBuffer));
				this->_response.append(imgBuffer, this->_file.gcount());				
			}
		}
		else {
			this->_response.append("Content-Type: " + this->_contentTypes[this->_contentType] + "\r\n\r\n");
			std::string line;
			while (std::getline(this->_file, line)) 
				this->_response.append(line);
			std::cout << RED << "RESPONSE:\n" << _response << CLEAR << std::endl;
		}

		if (_file.bad()) { throw std::runtime_error("Error"); }
	}
	catch (std::exception &e) {
		this->_response.clear();
		
		this->_response.append(ISE_500);
		std::string body = ISE_MESSAGE;
		std::cout << "500 body:\n" << body << std::endl;
		this->_contentLength = body.length();
		this->_response.append(std::to_string(this->_contentLength));
		this->_response.append(body);
	}
	this->_file.close();
}
