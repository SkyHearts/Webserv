/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 00:29:05 by jyim              #+#    #+#             */
/*   Updated: 2023/09/27 00:29:05 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "headers.hpp"

class autoindex
{
	private:
	std::map<std::string, std::string> _content;
	public:
	ServerConfig configinfo;
	autoindex(std::map<std::string, std::string> content) : _content(content), {}
	~autoindex();
	std::string generateList(ServerConfig portinfo);
	std::string getContentValue(std::string key);

};