/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePost.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:03 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/02 15:56:04 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "responseBase.hpp"

class ResponsePost : public ResponseBase {
	private:
		ResponsePost( void );

		std::string _requestBody, _boundary;
		std::map < std::string, std::string > _requestHeader;
		std::ifstream _file;

	public:
		ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, size_t payload, ServerConfig portinfo);
		~ResponsePost( void );

		void clearResources( void );
		void handleMultipartFormData( std::string filename, std::string rawData );
		void handleTextData( std::string requestBody );
		bool validateResource( const std::string &name );
		void saveData( void );
		void setStatusCodePost( int status, int isUpload );
		void generateResponse( void );
};