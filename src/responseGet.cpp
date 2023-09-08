/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:08:23 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/08 14:12:27 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseGet.hpp"

/*============================================================================*/
ResponseGet::ResponseGet( void ) : ResponseBase() { }

ResponseGet::ResponseGet( std::string filePath ) : ResponseBase() {
	_path.append(filePath);

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

static std::string fileExtension( const std::string& filename, const std::map< std::string, std::string > &types ) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos != std::string::npos && dotPos < filename.length() - 1) {
		std::string foundExtension = filename.substr(dotPos + 1);
		if (types.find(foundExtension) != types.end())
			return (foundExtension);
		else
			throw std::out_of_range("Invalid Extension");
	}

	throw std::runtime_error("Error");
}

//How do I check extensions without hardcoding?
// I can utilise the out_of_range exception thrown by map.at()
// but how do I do this ;-;
void ResponseGet::checkPath( void ) {
	_isImg = false;

	// if (_path == "/") {
	// 	_path.clear();
	// 	_path.append("html/index.html");
	// }
	// if (_path.empty())
	// 	_path.append("html/index.html");
	// 	//_path.append(root + "/" + index)
	// else if (hasExtension(_path, "png") != "") {
	// 	_isImg = true;
	// 	std::cout << _path << std::endl;
	// }
	// else
	// 	_path.erase(0,1);

	// _file.open(_path);
	// if (_file.is_open())
	// 	_statusCode = 200;
	// else
	// 	_statusCode = 404;

	// if (_statusCode != 200)
	// 	_file.open("html/" + std::to_string(_statusCode) + ".html");

	if (_path.empty())
		_path.append("html/index.html");
	else {
		try {
			_contentType = fileExtension(_path, _contentTypes);
			_statusCode = 200;
		}
		catch ( std::exception &e ) {
			std::cout << "invalid path" << std::endl;
			_statusCode = 404;
			_path.clear();
			_path.append("html/" + std::to_string(_statusCode) + ".html");
		}
	}

	if (_contentType == "png" || _contentType == "jpg" || _contentType == "jpeg")
		_isImg = true;

	_file.open(_path);
	if (!_file.is_open())
		_statusCode = 500;
}


void ResponseGet::generateResponse( void ) {
	_response.clear();

	if (_statusCode = 500) {
		_response.append(ISE_500);
		std::string body = std::string(ISE_HTML);
		_contentLength = body.length();
		_response.append(std::to_string(_contentLength));
		_response.append(ISE_HTML);
	}
	else {
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
}

std::string ResponseGet::getResponse( void ) const {
	return (_response);
}


/*
	root == / == "html"
	default file == index == "index.html"

	"root/default file" == html/index.html
*/