/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 00:34:00 by jyim              #+#    #+#             */
/*   Updated: 2023/09/27 00:34:00 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/autoindex.hpp"
#include "../includes/parse.hpp"

autoindex::autoindex( void ) { }

autoindex::autoindex( std::map<std::string, std::string> content ) : _content(content) {
	this->_response = getHead(this->_content["Path"]);
}

autoindex::~autoindex( void ) { }

std::string autoindex::getContentValue( std::string key ) {
	return(this->_content[key]);
}

std::string autoindex::getHead( std::string path ) { // ( ͡° ͜ʖ ͡°)
	std::fstream file;
	std::string head;
	std::string search = "%folder";
	file.open(autoIndexPath);
	if(file.is_open()) {
		std::string in;
		while (std::getline(file, in, '\n')) {
			in = trim(in);
			int pos = in.find(search);
			if(in.find(search) != std::string::npos)
				in.replace(pos, search.length(), path);
			if(in.find("<body>") != std::string::npos)
				break ;
			if(in.empty())
				continue ;
			head += in;
		}
	}
	else {
		std::cerr << RED << "autoindex open file error" << CLEAR << std::endl;
		return std::string();
	}
	file.close();
	return head; // ( ͡° ͜ʖ ͡°)
}

bool isValidDir( const char *path ) {
	struct stat info;
	if (stat(path, &info) == 0)
		return S_ISDIR(info.st_mode);
	else
		return 0;
}

bool isValidFile( const char *path ) {
	struct stat info;
	if (stat(path, &info) == 0)
		return S_ISREG(info.st_mode);
	else
		return 0;
}

std::string checkExt( std::string item, struct stat info ) {
	std::map<std::string, std::string> extensions;

	extensions["png"] = "PNG Image";
	extensions["jpg"] = "JPEG Image";
	extensions["svg"] = "SVG Image";
	extensions["gif"] = "GIF Image";
	extensions["ico"] = "Windows Icon";
	extensions["txt"] = "Text File";
	extensions["log"] = "Log File";
	extensions["html"] = "HTML File";
	extensions["php"] = "PHP Script";
	extensions["js"] = "Javascript";
	extensions["css"] = "Stylesheet";
	extensions["pdf"] = "PDF Document";
	extensions["zip"] = "ZIP Archive";
	extensions["bak"] = "Backup File";
	extensions["c"] = "C Programming  File";
	extensions["cpp"] = "CPP Programming  File";
	
	if (S_ISDIR(info.st_mode))
		return ("DIRECTORY");
	else if (S_ISREG(info.st_mode)) {
		std::string ext;
		size_t pos;
		if((pos = item.rfind('.')) != std::string::npos)
			ext = item.substr(pos+1);
		if (extensions.find(ext) != extensions.end())
			return (extensions[ext]);
		else
			return "UNKNOWN TYPE";
	}
	else
		return "UNKNOWN TYPE";
}

std::string autoindex::generateList(ServerConfig portinfo){
	// ./webserv is the actual starting directory represented by "./", then root and path is added in to get desired directory
	std::string path = "/" + getContentValue("Path");
	std::string referer = getContentValue("Referer");

	if (referer.find("http", 0, 4) == std::string::npos)
		referer = "http://" + referer;
	if (this->_response.empty())
		this->_response = getHead(path);

	std::string currentdir = "./" + portinfo.root + path;
	std::string rootdir = "./" + portinfo.root + uriRoot(path);
	DIR* dir = opendir(currentdir.c_str());
	if (!dir) {
		std::cout << "Directory" << currentdir << "not found" << std::endl;
		return "Directory " + currentdir + " not found";
	}

	struct stat info;
	struct dirent *item;

	this->_response.append("<body><div id=\"container\"><h1>Directory contents</h1><table class=\"sortable\">");
	this->_response.append("<body><div id=\"container\"><h3>Index of " + path + "</h3><table class=\"sortable\">");
	this->_response.append("<thead><tr><th>Filename</th><th>Type</th><th>Size <small>(bytes)</small></th><th>Date Modified</th></tr></thead><tbody>");

	while ((item = readdir(dir)) != NULL) {
		std::string file_path = currentdir + item->d_name;
		std::string row;
		std::string name(item->d_name);

		if (name.compare(".") == 0)
			continue;

		if (stat(file_path.c_str(), &info) == 0) {
			tm* currTm;
			char dateMod[100];
	
			currTm = localtime(&info.st_mtime);
			strftime(dateMod, 50, "%B %d, %Y %T", currTm);
			std::string url = path + item->d_name;

			if (isValidDir(file_path.c_str()))
				url += "/";

			this->_response.append("<tr class='$class'>");
			row = "<td><a href='" + url + "'>" + item->d_name + "</a></td>";
			if (name.compare("..") == 0)
				row += "<td><a href='" + url + "'>"+ "</a></td>";
			else
				row += "<td><a href='" + url + "'>" + checkExt(item->d_name, info) + "</a></td>";

			if (name.compare("..") == 0 || isValidDir(file_path.c_str()))
				row += "<td><a href='" + url + "'>" + "</a></td>";
			else
				row += "<td><a href='" + url + "'>" + std::to_string(info.st_size) + "</a></td>";

			row += "<td><a href='" + url + "'>" + dateMod + "</td></tr>";
		}
		this->_response.append(row);
	}
	this->_response.append("</tr></tbody></div></body></html>");

	closedir(dir);
	return this->_response;
}

std::string uriRoot( std::string str ) {
	std::string ret;
	size_t delim_pos = str.find_first_of("/");
	size_t len = str.find_first_of("/", delim_pos + 1);

	if (delim_pos != std::string::npos && len != std::string::npos)
		ret = str.substr(delim_pos, len + 1);
	else {
		std::cerr << RED << "uri root error" << CLEAR << std::endl;
		return std::string("empty");
	}

	return ret;
}

std::string trim( const std::string &str ) {
	size_t first = str.find_first_not_of('\t');

	if (std::string::npos == first)
		return str;

	size_t last = str.find_last_not_of('\t');
	return str.substr(first, (last - first + 1));
}