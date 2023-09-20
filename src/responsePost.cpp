#include "responsePost.hpp"

/*============================================================================*/
ResponsePost::ResponsePost( void ) : ResponseBase() { }

ResponsePost::ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, size_t payload) : ResponseBase() {
	clearResources();

	this->_path.append(filePath);
	this->_requestHeader = reqHead;
	this->_requestBody.append(reqBody);
	this->_payload = payload;

	std::cout << CLEAR << "\nRequest Body:\n[" << _requestBody << "]" << std::endl;
	checkPath(); //dir exists and permissions

	//set content type
	//check if file exists
	//createResource();
	generateResponse();
}

ResponsePost::~ResponsePost( void ) { }
/*============================================================================*/

void ResponsePost::clearResources( void ) {
	this->_fileName.clear();
	this->_requestHeader.clear();
	this->_requestBody.clear();
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

bool ResponsePost::checkPermissions( void ) {
	// something something _path
	return (false);
}

void ResponsePost::checkPath( void ) {
	struct stat pathStat;

	if (stat(this->_path.c_str(), &pathStat) == 0) {
		if (!checkPermissions())
			set405();

	}
	else {
		std::cout << "doesnt exist" << std::endl;
	}
}