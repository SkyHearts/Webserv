/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:08:33 by jyim              #+#    #+#             */
/*   Updated: 2023/09/06 19:09:07 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cgi_handler.hpp"

cgi_handler::cgi_handler(){
	_env = NULL;
	std::cout << "Default constructor" << std::endl;
}

cgi_handler::~cgi_handler( void ){
	std::cout << "Deconstruct Config" << std::endl;
	//delete env
	for(int i = 0; i < (sizeof(_env) / sizeof(char*)); ++i) {
        delete[] _env[i];   
    }
    //Free the array of pointers
    delete[] _env;
}

void cgi_handler::add_env(std::string envVar){
	int envSize;
	char** tmp_env;
	if (_env == NULL){
		_env = new char*[2];
		_env[0] = new char[envVar.length() + 1];
		//_env[0] = envVar.c_str();
		strcpy(_env[0], envVar.c_str());
		_env[1] = NULL;
	}
	envSize = (sizeof(_env) / sizeof(char*));
	tmp_env = new char*[envSize + 1];
	copyEnv(tmp_env, _env);
	strcpy(tmp_env[envSize - 1], envVar.c_str());
	tmp_env[envSize] = NULL;
	delEnv(_env);
	_env = tmp_env;

}

void cgi_handler::create_env(void){
	add_env("SERVER_SOFTWARE = WebServ");
	add_env("SERVER_PORT =" + );
}

std::string cgi_handler::exec_cgi(std::string path, char **argv){
	pid_t cgi;
	std::string response;
	cgi = fork();
	if (cgi == 0)
	{
		execve(path.c_str(), argv, this->_env);
	}
	return (response);
}

void cgi_handler::delEnv(char **env){
	for(int i = 0; i < (sizeof(env) / sizeof(char*)); ++i) {
		delete[] env[i];   
    }
    //Free the array of pointers
    delete[] env;
}