/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:08:23 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/02 10:37:43 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseGet.hpp"

/*============================================================================*/

ResponseGet::ResponseGet( void ) : ResponseBase() { }

ResponseGet::ResponseGet( std::string filePath, ServerConfig portinfo ) : ResponseBase() {
	_portinfo = portinfo;
	_autoindex = false;
	_path.clear();
	this->_path.append(filePath);

	checkPath();
	generateResponse();
}

ResponseGet::~ResponseGet( void ) { }

/*============================================================================*/

/*
	Find the file extension of a given filename
	- Find the last dot in the filename
	- If there is a dot and it is not the last character in the filename
	- - Return the extension
	- Else return "INVALID"
*/
static std::string fileExtension( const std::string& filename ) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos != std::string::npos && dotPos < filename.length() - 1) {
		std::string foundExtension = filename.substr(dotPos + 1);
		return (foundExtension);
	}
	return "";
}

/*
	Last character in requested path is '/'?
*/
bool isAutoIndex( const std::string path ) {
	char lastChar = path[path.length() - 1];
	return lastChar == '/';
}

/*
	Find the content type of a given file extension
	- If the extension is in the map
	- - Return the content type
*/
void ResponseGet::checkPath( void ) {
	this->_isImg = false;

	_path.erase(0, 1);

	if (!_path.empty() && isAutoIndex(_path)) {
		for (std::vector<Location>::iterator iter = _portinfo.locations.begin(); iter < _portinfo.locations.end(); iter++) {
			(*iter).uri.erase(0, 1);
			if (_path.find((*iter).uri) != std::string::npos) {
				if ((*iter).autoindex == true) {
					setContentType("html");
					_path.insert(0, "/");
					this->_autoindex = true;
				}
			}
		}
	}

	if (this->_path.empty()) {
		setContentType("html");
		this->_path.append(_portinfo.root + "/" + _portinfo.index);
	}
	else if (!this->_path.empty() && !_autoindex) {
		setContentType(fileExtension(this->_path));
		if (this->_contentType == "png" || this->_contentType == "jpg" || this->_contentType == "jpeg" || this->_contentType == "ico") {
			this->_isImg = true;
			if (_path.find("assets/") == std::string::npos)
            	_path.insert(0, _portinfo.root + "/");
        }
		else if (this->_contentType == "html") {
			setContentType("plain");
             _path.insert(0, _portinfo.root + "/");
		}
        else if (this->_contentType == "txt") {
            setContentType("plain");
            _path.insert(0, _portinfo.root + "/");
        }
		else if (this->_contentType == "") {
			setContentType("html");
			_path.insert(0, "/");
            std::cout << "Path is: " << this->_path << std::endl;
			for (std::vector<Location>::iterator iter = _portinfo.locations.begin(); iter < _portinfo.locations.end(); iter++) {
				if (this->_path.find((*iter).uri) != std::string::npos) {
					_path.clear();
					if (!(*iter).index.empty())
						this->_path.append(_portinfo.root + (*iter).uri + "/" + (*iter).index);
				}
			}
		}
	}

	setStatusCodeGet();
}

/*
	Set the status code of a GET request
	- If the file is found and the content type is valid
	- - Set the status code to 200
	- Else
	- - Set the status code to 404
*/
void ResponseGet::setStatusCodeGet( void ) {
	if (!checkPermissions("GET")) {
		setStatusCode(405);
		this->_file.open(this->_portinfo.errorPages[405]);
	}
	else if (_autoindex == true)
		return ;
	else if ((this->_contentTypes.find(this->_contentType) != this->_contentTypes.end())) {
		if (this->_path == this->_portinfo.errorPages[501]) setStatusCode(501);
		else setStatusCode(200);

		this->_file.open(this->_path);
	}

	if (!this->_file.is_open()) {
		setStatusCode(404);
		setContentType("html");
		_file.open(_portinfo.errorPages[404]);
	}
}

/*
	Generate the response for a GET request
	- Append the content type to the response
	- Append the requested file contents to the response
	- If the file is bad, throw an exception
	- Close the file
*/
void ResponseGet::generateResponse( void ) {
	this->_response.clear();

	try {
		if (_autoindex) {
			std::map< std::string, std::string > content;
			content["Path"] = this->_path;
			content["Referer"] = _portinfo.name + ":" + std::to_string(_portinfo.listen);

			autoindex ai(content);
			_response.clear();
			_response.append("HTTP/1.1 200 OK\r\n");
			_response.append("Content-Type: text/html\r\n\r\n");

			_response += ai.generateList(_portinfo);
		}
		else {
			this->_response.append("HTTP/1.1 " + std::to_string(this->_statusCode) + " " + this->_statusCodes[this->_statusCode] + "\r\n");

			if (this->_isImg) {
				this->_response.append("Content-Type: " + this->_contentTypes[this->_contentType] + "\r\n\r\n");

				char *imgBuffer = new char[1024];
				std::memset(imgBuffer, 0, 1024);
				while (!this->_file.eof()) {
					this->_file.read(imgBuffer, 1024);
					this->_response.append(imgBuffer, this->_file.gcount());				
				}
				delete [] imgBuffer;
			}
			else {
				this->_response.append("Content-Type: " + this->_contentTypes[this->_contentType] + "\r\n\r\n");
				std::string line;
				while (getline(this->_file, line)) 
					this->_response.append(line);
			}

			if (_file.bad()) { throw std::runtime_error("Error"); }
		}
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

	_file.close();
}