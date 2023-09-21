#include "responsePost.hpp"

/*============================================================================*/
ResponsePost::ResponsePost( void ) : ResponseBase() { }

ResponsePost::ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, size_t payload, ServerConfig portinfo) : ResponseBase() {
	clearResources();

	this->_portinfo = portinfo;
	this->_path.append(filePath);
	this->_requestHeader = reqHead;
	this->_requestBody.append(reqBody);
	this->_payload = payload;

	if (checkPermissions("POST")) {
		saveData();
		//createResource();
	}
	generateResponse();
}

ResponsePost::~ResponsePost( void ) { }
/*============================================================================*/

void ResponsePost::clearResources( void ) {
	this->_fileName.clear();
	this->_requestHeader.clear();
	this->_requestBody.clear();
}

void ResponsePost::saveData( void ) {
	std::string search = _requestHeader["Content-Type"];
	this->_boundary = search.substr(search.find("boundary=") + 1);

	std::ofstream tempFile;
	std::istringstream formBody(this->_requestBody);
	std::string line, temp, key, value;
	std::map< std::string, std::string > formHead;

	tempFile.open(this->_portinfo.root + "/rawData");
	if (!this->_boundary.empty())
		std::getline(formBody, line);
	while (getline(formBody, line)) {
		if (line == "\r\n\r\n")
			break;
		try {
			key = line.substr(0, line.find(':'));
			value = line.substr(line.find(':') + 2);
			std::cout.flush();
		}
		catch (std::exception const &e) {
			break ;
		}
		formHead.insert(std::pair< std::string, std::string >(key, value));
	}
	
	getline(formBody, line, '\n');
	temp.append(line);
	while (getline(formBody, line, '\n') && !line.empty()) {
		tempFile << temp;
		if (line == _boundary)
			break ;
		if (line != _boundary)
			tempFile << "\n";
		temp.clear();
		temp.append(line);
	}

	std::cout << RED << "FORM HEAD AND BODY" << std::endl;
	for (std::map<std::string, std::string>::iterator it = formHead.begin(); it != formHead.end(); it++)
		std::cout << "Key: " << (*it).first << " | Value: " << (*it).second << std::endl;

	std::cout << "\nBODY:\n" << std::endl;
	std::cout << _requestBody << CLEAR << std::endl;
}

void ResponsePost::set405( void ) {
	std::ifstream file;

	file.open("html/405.html");
	_response.clear();

	// try {
	// 	_response.append("HTTP/1.1 405 Not Found\r\n");
	// 	_response.append("Content-Type: text/html\r\n\r\n");
		
	// 	std::string line;
	// 	while (std::getline(file, line))
	// 		_response.append(line);
	// }
}

void ResponsePost::generateResponse( void ) {
	_response.append("bla");
}

