/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePost.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:03 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/21 15:32:47 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "responseBase.hpp"

class ResponsePost : public ResponseBase{
	private:
		ResponsePost( void );
		void setContentType( std::string );

		std::string _requestBody, _boundary;
		std::map < std::string, std::string > _requestHeader;

	public:
		ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, size_t payload, ServerConfig portinfo);
		~ResponsePost( void );

		void clearResources( void );
		void saveData( void );
		void set405( void );
		void generateResponse( void );
};