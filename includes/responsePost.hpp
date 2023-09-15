/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePost.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:03 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/15 14:01:13 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "responseBase.hpp"

typedef struct fileData {
	std::map< std::string, std::string > header;
}	fileData;

class ResponsePost : public ResponseBase{
	private:
		ResponsePost( void );
		void setContentType( std::string );

		std::string _requestBody, _boundary;
		std::vector< fileData > _formData;
		std::map < std::string, std::string > _requestHeader;

	public:
		ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, size_t payload);
		~ResponsePost( void );

		void clearResources( void );
		void createResource( void );
		void checkPath( void );
		bool checkPermissions( void );
		void set405( void );
		void generateResponse( void );
};