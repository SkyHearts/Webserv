/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePost.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:03 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/13 15:49:38 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "responseBase.hpp"

class ResponsePost : public ResponseBase{
	private:
		ResponsePost( void ); //unused
		void setContentType( void );

		std::string _fileName;
		std::map < std::string, std::string > _requestHeader;
		std::string _requestBody;

	public:
		ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody);
		~ResponsePost( void );

		void clearResources( void );
		void checkPath( void );
		void generateResponse( void );
};