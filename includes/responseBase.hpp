/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseBase.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:37:55 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/29 14:52:21 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"

class ResponseBase {
	protected:
		std::string _response;
		std::string _contentType;
		std::size_t _contentLength;
		std::string _path;
		std::string _fileName;
		size_t _payload;
		int _statusCode;
		bool _isImg;
		std::map< std::string, std::string > _contentTypes;
		std::map< int, std::string > _statusCodes;
		ServerConfig _portinfo;

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
		std::string getResponse( void );
		bool checkPermissions( std::string method );
		virtual void generateResponse( void ) = 0;
		std::string generateResponseISE( void );

};