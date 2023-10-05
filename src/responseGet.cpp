/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:08:23 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/05 15:36:25 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseGet.hpp"

/*============================================================================*/

ResponseGet::ResponseGet( void ) : ResponseBase() { }

ResponseGet::ResponseGet( std::string filePath, ServerConfig portinfo ) : ResponseBase() {
	resetResources();
	
	this->_portinfo = portinfo;
	this->_path = decodeEncoding(filePath);

	if (this->_path == "unknown")
		this->_unknown = true;
	checkPath();
	generateResponse();
}

ResponseGet::~ResponseGet( void ) { }

/*============================================================================*/

void ResponseGet::resetResources( void ) {
	this->_autoindex = false;
	this->_unknown = false;
}

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

/* Last character in requested path is '/'? */
bool isAutoIndex( const std::string path ) {
	char lastChar = path[path.length() - 1];
	return lastChar == '/';
}

/*
		If path is not root AND path ends with a '/'
		Loop through each stored location (uri)
		Erase the leading '/' from the uri since _path already has leading '/' removed
		If _path match with a uri, check if autoindex for that uri is on
		If is on, set autoindex to true

		Appends the path to root, unless it's an image file.
		If no content type specified (aka directory), append path to respective html page.
*/
void ResponseGet::checkPath( void ) {
	this->_isImg = false;

	this->_path.erase(0, 1);
	if (!this->_path.empty() && isAutoIndex(this->_path)) {
		for (std::vector<Location>::iterator iter = this->_portinfo.locations.begin(); iter < this->_portinfo.locations.end(); iter++) {
			std::string tmp = (*iter).uri;
			tmp.erase(0, 1);
			if (this->_path.find(tmp) != std::string::npos) {
				if ((*iter).autoindex == true) {
					setContentType("html");
					this->_autoindex = true;
				}
			}
		}
	}

	if (this->_path.empty() && !this->_unknown) {
		setContentType("html");
		this->_path.append(this->_portinfo.root + "/" + this->_portinfo.index);
	}
	else if (!this->_path.empty() && !this->_autoindex) {
		setContentType(fileExtension(this->_path));
		if (this->_contentType == "png" || this->_contentType == "jpg" || this->_contentType == "jpeg" || this->_contentType == "ico") {
			this->_isImg = true;
			if (this->_path.find("assets/") == std::string::npos)
            	this->_path.insert(0, this->_portinfo.root + "/");
        }
		else if (this->_contentType == "html"){
             this->_path.insert(0,this->_portinfo.root + "/");
		}
        else if (this->_contentType == "txt") {
            setContentType("plain");
            this->_path.insert(0, this->_portinfo.root + "/");
        }
		else if (this->_contentType == "") {
			setContentType("html");
			this->_path.insert(0, "/");
			if (this->_unknown) {
				this->_path.clear();
				this->_path.append(this->_portinfo.errorPages[501]);
			}
			else {
				for (std::vector<Location>::iterator iter = this->_portinfo.locations.begin(); iter < this->_portinfo.locations.end(); iter++) {
					if (this->_path == (*iter).uri) {
						this->_path.clear();
						if (!(*iter).index.empty())
							this->_path.append(this->_portinfo.root + (*iter).uri + "/" + (*iter).index);
					}
				}
			}
		}
	}
	setStatusCodeGet();
}

/*
	Set the status code of a GET request
	- If the file is found and the content type is valid
	- Set the status code to 200
	- Else
	- - Set the status code to 404
*/
void ResponseGet::setStatusCodeGet( void ) {
	if (_autoindex == true)
		return ;
	else if ((this->_contentTypes.find(this->_contentType) != this->_contentTypes.end()))
		this->_file.open(this->_path);

	if (!this->_file.is_open()) {
		setStatusCode(404);
		this->_file.close();
		this->_file.open(_portinfo.errorPages[404]);
	}
	else if (!this->_unknown && !checkPermissions("GET")) {
		setStatusCode(405);
		this->_file.close();
		this->_file.open(this->_portinfo.errorPages[405]);
	}
	else if (this->_unknown)
		setStatusCode(501);
	else
		setStatusCode(200);
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
		if (this->_autoindex) {
			std::map< std::string, std::string > content;
			content["Path"] = this->_path;
			content["Referer"] = this->_portinfo.name + ":" + std::to_string(this->_portinfo.listen);

			autoindex ai(content);
			this->_response.clear();
			this->_response.append("HTTP/1.1 200 OK\r\n");
			this->_response.append("Content-Type: text/html\r\n\r\n");

			this->_response += ai.generateList(this->_portinfo);
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
			if (this->_file.bad())
				throw std::runtime_error("Error");
		}
	}
	catch (std::exception &e) {
		this->_response.clear();
		this->_response.append(generateResponseISE());
	}
	this->_file.close();
}