#include "responsePost.hpp"

/*============================================================================*/
ResponsePost::ResponsePost( void ) : ResponseBase() { }

ResponsePost::ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody) : ResponseBase() {
	clearResources();

	this->_path.append(filePath);
	this->_requestHeader = reqHead;
	this->_requestBody.append(reqBody);

	std::cout << CLEAR << std::endl;
	checkPath(); //dir exists and permissions

	//set content type
	//check if file exists
	generateResponse();
}

ResponsePost::~ResponsePost( void ) { }
/*============================================================================*/

void ResponsePost::clearResources( void ) {
	this->_fileName.clear();
	this->_requestHeader.clear();
	this->_requestBody.clear();
}

void ResponsePost::generateResponse( void ) {
	this->_response.append("wawawa");
}

void ResponsePost::checkPath( void ) {
	struct stat pathStat;

	if (stat(this->_path.c_str(), &pathStat) == 0) {
		std::cout << "exists" << std::endl;
	}
	else {
		std::cout << "doesnt exist" << std::endl;
	}
}