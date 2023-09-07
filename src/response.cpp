#include "response.hpp"

/*============================================================================*/
Response::Response( void ) { }

Response::~Response( void ) { }
/*============================================================================*/

void Response::initStatusCodes( void ) {
	_statusCodes[200] = "OK";
	_statusCodes[201] = "Created";
	_statusCodes[202] = "Accepted";
	_statusCodes[404] = "Not Found";
	_statusCodes[405] = "Method Not Allowed";
	_statusCodes[413] = "Payload Too Large";
	_statusCodes[501] = "Not Implemented";
}

// void Response::initContentTypes( void ) {
// 	_contentTypes["html"] = "text/html";
// 	_contentTypes["css"] = "text/css";
// 	_contentTypes["js"] = "text/javascript";
// 	_contentTypes["png"] = "image/png";
// 	_contentTypes["jpg"] = "image/jpeg";
// 	_contentTypes["jpeg"] = "image/jpeg";
// 	_contentTypes["ico"] = "image/x-icon";
// }

std::string Response::getResponse( void ) const {
	return (_response);
}