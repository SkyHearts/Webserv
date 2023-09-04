/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:36:28 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/04 13:56:17 by nnorazma         ###   ########.fr       */
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
	while (getline(request, line, '\n')) {
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

void Request::setStatusCode( void ) {
	_statusCode = 200;
}

void Request::setResponse( void ) {
	std::ifstream file;

	file.open("html/default.html");
	if (!file.is_open()) {
		std::cerr << "Error opening file." << std::endl;
		return ;
	}

	_response.clear();
	_response.append("HTTP/1.1 " + std::to_string(_statusCode) + " " + _statusCodes[_statusCode] + "\r\n");
	_response.append("Content-Type: text/html\r\n\r\n");

	std::string line;
	while (std::getline(file, line)) {
		_response.append(line);
	}
	file.close();
}

std::string Request::processRequest( std::string req ) {
	_request = req;
	parseRequest();
	//filter error
	setStatusCode();
	setResponse(); //default atm

	return _response;
}