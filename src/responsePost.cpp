#include "responsePost.hpp"

/*============================================================================*/
ResponsePost::ResponsePost( void ) : ResponseBase() { }

ResponsePost::ResponsePost( std::string filePath ) : ResponseBase() {
	_path.append(filePath);
}

ResponsePost::~ResponsePost( void ) { }
/*============================================================================*/