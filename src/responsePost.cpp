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
	std::string line;
	tempFile.open(root + "/rawData");
	if (!this->_boundary.empty()) {
		//bound\r\n
		//head\r\n
		//\r\n
		//body\r\n
		//bound
	}
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
