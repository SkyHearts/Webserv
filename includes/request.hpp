/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:37:14 by nnorazma          #+#    #+#             */
/*   Updated: 2023/08/29 15:02:58 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"

enum request {
	GET,
	POST,
	DELETE,
	UNKNOWN
};

class Request {
	private:
		int	_headSize;
		std::string _method;
		std::string _path;
		std::string _http;
		std::map< std::string, std::string > _content;

	public:
		Request( void );
		~Request( void );
		void parseRequest( std::string request );
};