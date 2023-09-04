/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:08:33 by jyim              #+#    #+#             */
/*   Updated: 2023/09/04 17:10:59 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cgi_handler.hpp"

cgi_handler::cgi_handler(){
	//USE DEFAULT CONF FILE if there are no files specified
	std::cout << "Default constructor" << std::endl;
}

cgi_handler::~cgi_handler( void ){
	std::cout << "Deconstruct Config" << std::endl;
}