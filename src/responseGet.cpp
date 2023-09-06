/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:08:23 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/06 19:00:58 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseGet.hpp"

/*============================================================================*/
ResponseGet::ResponseGet( void ) { }

ResponseGet::ResponseGet( std::string filePath ) : Response(), _path(filePath) {
	checkPath();
	generateResponse();
}

ResponseGet::~ResponseGet( void ) { }
/*============================================================================*/

void ResponseGet::setContentType( void ) {

}

static std::string hasExtension( const std::string& filename, const std::string& extension ) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos != std::string::npos && dotPos < filename.length() - 1) {
		std::string foundExtension = filename.substr(dotPos + 1);
		if (foundExtension == extension)
			return foundExtension;
	}

	return "";
}

void ResponseGet::checkPath( void ) {
	_isImg = false;

	if (_path == "/") {
		_path.clear();
		_path.append("html/index.html");
	}
	else if (hasExtension(_path, "png") != "") {
		_isImg = true;
		_path.erase(0,1);
		std::cout << _path << std::endl;
	}
	else
		_path.erase(0,1);

	_file.open(_path);
	if (_file.is_open())
		_statusCode = 200;
	else
		_statusCode = 404;

	if (_statusCode != 200)
		_file.open("html/" + std::to_string(_statusCode) + ".html");
}


void ResponseGet::generateResponse( void ) {
	_response.clear();
	_response.append("HTTP/1.1 " + std::to_string(_statusCode) + " " + _statusCodes[_statusCode] + "\r\n");

	if (_isImg) {
		_response.append("Content-Type: image/png\r\n\r\n");

		char img_buffer[1024];
		while (!_file.eof()) {
			_file.read(img_buffer, sizeof(img_buffer));
			_response.append(img_buffer, _file.gcount());
		}
	}
	else {
		_response.append("Content-Type: text/html\r\n\r\n");

		std::string line;
		while (std::getline(_file, line))
			_response.append(line);
	}

	_file.close();
}

std::string ResponseGet::getResponse( void ) const {
	return (_response);
}
