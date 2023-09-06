/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:00 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/06 16:30:17 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"

class responseGet {
	private:
		responseGet( void ); //unused

		int _statusCode;
		std::string _path;
		std::string _contentType;
		std::string _response;
		std::map< std::string, std::string > _contentTypes;

		void contentTypeInit( void );
		void setContentType( void );
		void checkPath( void );

	public:
		responseGet( std::string filePath );
		~responseGet( void );

		void generateResponse( void );
		std::string getResponse( void ) const;
};