/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:00 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/06 19:03:36 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "response.hpp"

class ResponseGet : public Response {
	private:
		ResponseGet( void ); //unused

		int _statusCode;
		std::ifstream _file;
		std::string _path;

		void setContentType( void );
		void checkPath( void );

	public:
		ResponseGet( std::string filePath );
		~ResponseGet( void );

		void generateResponse( void );
		std::string getResponse( void ) const;
};