/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:00 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/15 12:57:51 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "responseBase.hpp"

class ResponseGet : public ResponseBase {
	private:
		ResponseGet( void ); //unused
		std::ifstream _file;

	public:
		ResponseGet( std::string filePath );
		~ResponseGet( void );

		void clearResources( void );
		void checkPath( void );
		void setStatusCodeGet( void );
		void generateResponse( void );
};