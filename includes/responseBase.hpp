#pragma once
#include "headers.hpp"

//this is disgusting
#define ISE_500 "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\nContent-Length: "
#define ISE_HTML "\r\n\r\n<!DOCTYPE html><html><head><title>Internal Server Error.</title></head><body><h1>500 Internal Server Error</h1><p>The Trinity Server has encountered an error and was unable to complete your request.</p></body></html>"

class ResponseBase {
	protected:
		std::string _response;
		std::string _contentType;
		std::size_t _contentLength;
		std::string _path;
		int _statusCode;
		bool _isImg;
		std::map< std::string, std::string > _contentTypes;
		std::map< int, std::string > _statusCodes;

	public:
		ResponseBase( void );
		virtual ~ResponseBase( void );

		void initStatusCodes( void );
		void initContentTypes( void );
		virtual void generateResponse( void ) = 0;
		std::string getResponse( void ) const;
		std::string getPath( void ) const { return _path; }
		void setContentType ( std::string type );
		void setStatusCode ( int code );
		std::map< std::string, std::string > getContentTypes( void ) const;
		std::map< int, std::string > getStatusCodes( void ) const;
};