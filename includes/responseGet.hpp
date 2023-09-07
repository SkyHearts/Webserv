/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:00 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/07 12:19:48 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "responseBase.hpp"

class ResponseGet : public ResponseBase {
	private:
		std::ifstream _file;

		ResponseGet( void ); //unused
		void setContentType( void );
		void checkPath( void );

	public:
		ResponseGet( std::string filePath );
		~ResponseGet( void );

		void generateResponse( void );
		std::string getResponse( void ) const;
};