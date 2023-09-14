/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePost.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:03 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/14 16:03:32 by nnorazma         ###   ########.fr       */
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
		ResponsePost( void ); //unused
		void setContentType( void );

		std::string _requestBody;
		std::vector< fileData > _formData;
		std::map < std::string, std::string > _requestHeader;

	public:
		ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, size_t payload);
		~ResponsePost( void );

		void clearResources( void );
		void createResource( void );
		void checkPath( void );
		void generateResponse( void );
};