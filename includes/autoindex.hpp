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

#define autoIndexPath "./default/autoindex.html"

class autoindex
{
	private:
	std::map<std::string, std::string> _content;
    std::string _response;
	public:
	// ServerConfig configinfo;
	autoindex();
	autoindex(std::map<std::string, std::string> content);
	~autoindex();
	std::string generateList(ServerConfig portinfo);
	std::string getContentValue(std::string key);
    std::string getHead(std::string path);

};

std::string uriRoot(std::string str, std::string delim);
std::string trim(const std::string& str);