#include "responsePost.hpp"

/*============================================================================*/
ResponsePost::ResponsePost( void ) : ResponseBase() { }

ResponsePost::ResponsePost( std::string filePath ) : ResponseBase() {
	this->_path.append(filePath);

	checkPath();
	generateResponse();
}

ResponsePost::~ResponsePost( void ) { }
/*============================================================================*/

void ResponsePost::generateResponse( void ) {

}

void ResponsePost::checkPath( void ) {


}