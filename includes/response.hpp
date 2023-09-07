#pragma once
#include "headers.hpp"

class Response {
	protected:
		std::string _response;
		std::string _contentType;
		int _statusCode;
		bool _isImg;
		// std::map< std::string, std::string > _contentTypes;
		std::map< int, std::string > _statusCodes;

	public:
		Response( void );
		virtual ~Response( void );

		void initStatusCodes( void );
		void initContentTypes( void );
		virtual void generateResponse( void ) = 0;
		virtual std::string getResponse( void ) const;
};