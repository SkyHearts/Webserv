/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePost.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m4rrs <m4rrs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:03 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/04 23:44:05 by m4rrs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "responseBase.hpp"

class ResponsePost : public ResponseBase {
	private:
		ResponsePost( void );

		std::ifstream _file;
		std::string _requestBody, _boundary;
		std::map < std::string, std::string > _requestHeader;

	public:
		ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, ServerConfig portinfo);
		~ResponsePost( void );

		void resetResources( void );
		void createResource( const std::string &filename, std::string &rawData );
		void saveData( void );
		void setStatusCodePost( int status, int isUpload );
		void handleMultipartFormData( std::string filename, std::string rawData );
		void handleTextData( std::string requestBody );
		void generateResponse( void );
		
		bool validateResource( const std::string &name );
};