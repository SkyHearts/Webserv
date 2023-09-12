/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:37:14 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/12 18:05:48 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"
#include "responseGet.hpp"
#include "responsePost.hpp"
#include "responseDelete.hpp"
#include "responseUnknown.hpp"

class Request {
	private:
		std::string _request;
		std::string _response;
		std::string _method;
		std::string _path;
		std::string _http;
		std::string _contentBody;
		std::map< std::string, std::string > _content;


	public:
		Request( void );
		~Request( void );

		void parseRequest( void );
		std::string processRequest( std::string req );
};