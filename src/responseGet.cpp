/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:08:23 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/06 16:31:36 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseGet.hpp"

/*============================================================================*/
responseGet::responseGet( void ) { }

responseGet::responseGet( std::string filePath ) : _path(filePath) {
	checkPath(_path);
}

responseGet::~responseGet( void ) { }
/*============================================================================*/

void responseGet::contentTypeInit( void ) {
	_contentTypes["html"] = "text/html";
	_contentTypes["css"] = "text/css";
	_contentTypes["js"] = "text/javascript";
	_contentTypes["png"] = "image/png";
	_contentTypes["jpg"] = "image/jpeg";
	_contentTypes["jpeg"] = "image/jpeg";
	_contentTypes["ico"] = "image/x-icon";
}

void responseGet::setContentType( void ) {

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

void responseGet::checkPath( void ) {
	
	if (_path == "/") {
		_path.clear();
		_path.append("html/defaul.html");
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
	
	std::cout << "STATUS CODE: " << _statusCode << std::endl;
}

std::string responseGet::getResponse( void ) {
	return (_response);
}
