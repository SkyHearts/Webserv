/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:00 by nnorazma          #+#    #+#             */
/*   Updated: 2023/10/05 17:10:15 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"
#include "responseBase.hpp"
#include "autoindex.hpp"

class ResponseGet : public ResponseBase {
	private:
		ResponseGet( void );
		std::ifstream _file;
		bool _autoindex, _unknown, _redir;
		std::string _redirLocation;

	public:

		ResponseGet( std::string filePath, ServerConfig portinfo );
		~ResponseGet( void );

		void checkPath( void );
		void resetResources( void );
		void setStatusCodeGet( void );
		void generateResponse( void );
};