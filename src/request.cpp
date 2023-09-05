/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/05 15:05:18 by nnorazma         ###   ########.fr       */
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

void Request::checkPath( void ) {
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

void Request::setResponse( void ) {
	_response.clear();
	_response.append("HTTP/1.1 " + std::to_string(_statusCode) + " " + _statusCodes[_statusCode] + "\r\n");
	_response.append("Content-Type: text/html\r\n\r\n");

	std::string line;
	while (std::getline(_file, line)) {
		_response.append(line);
	}
	_file.close();
}

std::string Request::processRequest( std::string req ) {
	_request = req;
	parseRequest();
	
	if (_method == "GET") {
		responseGet get;
		_response = get.getResponse();
	}
	else if (_method == "POST") {

	}
	else if (_method == "DELETE") {

	}
	checkPath();
	setResponse();

	return _response;
}