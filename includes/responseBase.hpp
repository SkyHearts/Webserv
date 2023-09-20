/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseBase.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:37:55 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/14 16:51:28 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"

//this is disgusting
#define ISE_500 "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: "
#define ISE_MESSAGE "\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<title>Internal Server Error.</title>\n</head>\n<body>\n<h1>500 Internal Server Error</h1>\n<p>The Trinity Server has encountered an error and was unable to complete your request.</p>\n</body>\n</html>"

class ResponseBase {
	protected:
		std::string _response;
		std::string _contentType;
		std::size_t _contentLength;
		std::string _path;
		std::string _prevPath; //my brain can't think of a better way.
		std::string _fileName;
		size_t _payload;
		int _statusCode;
		bool _isImg;
		std::map< std::string, std::string > _contentTypes;
		std::map< int, std::string > _statusCodes;

	public:
		ResponseBase( void );
		virtual ~ResponseBase( void );

		void initStatusCodes( void );
		void initContentTypes( void );
		bool checkPermissions( std::string method, std::string path, ServerConfig portinfo );
		std::string getPath( void ) const { return _path; }
		std::map< std::string, std::string > getContentTypes( void ) const;
		std::map< int, std::string > getStatusCodes( void ) const;
		void setContentType ( std::string type );
		void setStatusCode ( int code );
		virtual void generateResponse( void ) = 0;
		std::string getResponse( void );
};