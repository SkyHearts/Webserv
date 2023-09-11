/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/11 16:00:48 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

/*============================================================================*/

Request::Request( void ) {
	initStatusCodes();
}

Request::~Request( void ) {}

/*============================================================================*/

void Request::initStatusCodes( void ) {
	_statusCodes[200] = "OK";
	_statusCodes[404] = "Not Found";
	_statusCodes[405] = "Method Not Allowed";
	_statusCodes[413] = "Payload Too Large";
	_statusCodes[501] = "Not Implemented";
}

void Request::parseRequest( void ) {
	_request.erase(remove(_request.begin(), _request.end(), '\r'), _request.end()); //line break in request is \r\n, this removes \r

	std::stringstream request(_request);
	std::string line, key, value;

	getline(request, line);
	std::istringstream head(line);
	head >> _method >> _path >> _http;

	_headSize = 0;
	while (getline(request, line, '\n') && !line.empty()) {
		try {
			key = line.substr(0, line.find(':'));
			value = line.substr(line.find(':') + 2);
			std::cout.flush();
		}
		catch (std::exception const &e) {
			break ;
		} 
		_content.insert(std::pair< std::string, std::string >(key, value));
		_headSize++;
	}
}

static bool hasExtension(const std::string& filename, const std::string& extension) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos != std::string::npos && dotPos < filename.length() - 1) {
		std::string foundExtension = filename.substr(dotPos + 1);
		return foundExtension == extension;
	}

	return false;
}

void Request::checkPath( void ) {
	_isImg = false;

	std::cout << "PATH = " << _path << std::endl;
	if (_path == "/") {
		_path.clear();
		_path.append("html/index.html");
	}
	else if (hasExtension(_path, "png")) {
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
	
	std::cout << "STATUS CODE: " << _statusCode << std::endl;
}

void Request::setResponse( void ) {
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

std::string Request::processRequest( std::string req ) {
	_request = req;
	std::cout << RED << "REQUEST\n" << req << CLEAR;
	parseRequest();
	checkPath();
	setResponse();

	return _response;
}