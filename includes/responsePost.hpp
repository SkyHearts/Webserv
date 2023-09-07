/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePost.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:06:03 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/05 15:13:59 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "headers.hpp"

class responsePost {
	private:
		std::string _response;
		int _statusCode;

	public:
		responsePost( void );
		~responsePost( void );

		std::string getResponse( void );
};