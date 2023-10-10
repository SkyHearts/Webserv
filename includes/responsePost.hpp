/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePost.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:03 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/05 15:20:29 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "responseBase.hpp"

class ResponsePost : public ResponseBase {
	private:
		ResponsePost( void );

		std::string								_requestBody, _boundary;
		std::map < std::string, std::string >	_requestHeader;
		std::ifstream							_file;
		bool		 							_usingCGI;

	public:
		ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, ServerConfig portinfo);
		~ResponsePost( void );

		void resetResources( void );
		void createResource( const std::string &filename, std::string &rawData );
		void saveData( void );
		void setStatusCodePost( int status, int isUpload );
		void handleTextData( std::string requestBody );
		void handleMultipartFormData( std::string filename, std::string rawData );
		void handleCalc( std::string requestBody );
		void generateResponse( void );

		bool validateResource( const std::string &name );
};