/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGet.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:00 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/05 15:13:39 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"

class responseGet {
	private:
		std::string _response;
		int _statusCode;

	public:
		responseGet( void );
		~responseGet( void );

		std::string getResponse( void );
};