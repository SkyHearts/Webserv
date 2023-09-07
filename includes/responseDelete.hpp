/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseDelete.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:00 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/06 16:12:39 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"

class responseDelete {
	private:
		std::string _response;
		int _statusCode;

	public:
		responseDelete( void );
		~responseDelete( void );

		std::string getResponse( void );
};