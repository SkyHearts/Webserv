/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:08:33 by jyim              #+#    #+#             */
/*   Updated: 2023/09/15 16:14:49 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cgi_handler.hpp"

int getCharDArraySize(char** array){
	int i = 0;
	while (array[i] != NULL)
		i++;
	return (i);
}

cgi_handler::cgi_handler() : _env(NULL), _arg(NULL){
	std::cout << "Default constructor" << std::endl;
}

cgi_handler::~cgi_handler( void ){
	std::cout << "Deconstruct Config" << std::endl;
}

void cgi_handler::reassginDArray(char **dest, char **src){
	for (int i = 0; i < getCharDArraySize(src); ++i)
		dest[i] = src[i];
	delete[] src;		
}

void cgi_handler::delDArray(char **dArray){
	for(int i = 0; i < getCharDArraySize(dArray); ++i) {
		delete[] dArray[i];   
    }
    delete[] dArray;
}

void cgi_handler::addEnv(std::string envVar){
	int envSize;
	char** tmp_env;
	if (_env == NULL){
		_env = new char*[2];
		_env[0] = strdup(envVar.c_str());
		_env[1] = NULL;
	}
	else{
		envSize = getCharDArraySize(_env);
		tmp_env = new char*[envSize + 2];
		reassginDArray(tmp_env, _env);
		tmp_env[envSize] = strdup(envVar.c_str());
		tmp_env[envSize + 1] = NULL;
		_env = tmp_env;
	}
}

void cgi_handler::createEnv(std::map<std::string, std::string> content){
	uri http;
	
	addEnv("SERVER_SOFTWARE = WebServ");
	addEnv("SERVER_PORT =");// + getPort() from parse request
	addEnv("REQUEST_METHOD =");// + getMethod() from parse request
	addEnv("PATH_INFO =");
	addEnv("PATH_TRANSLATED =");
	addEnv("HTTP_REFERER =");
	addEnv("HTTP_ACCEPT =");
}

void cgi_handler::addArg(std::string arg){
	int argSize;
	char** tmp_arg;
	if 	(_arg == NULL){
		_arg = new char*[2];
		_arg[0] = strdup(arg.c_str());
		_arg[1] = NULL;
	}
	else{
		argSize = getCharDArraySize	(_arg);
		tmp_arg = new char*[argSize + 2];
		reassginDArray(tmp_arg, _arg);
		tmp_arg[argSize] = strdup(arg.c_str());
		tmp_arg[argSize + 1] = NULL;
		_arg = tmp_arg;
	}
}

void cgi_handler::createArg(std::string path){
	addArg(path);
	// parse extra argument if exist
	//while (argu)
	//	addArg("");
}


void cgi_handler::execCGI(std::map<std::string, std::string> content, std::string path){
	pid_t pid;
	std::string response;
	
	createEnv(content);
	createArg(path);
	//// Print env
	for(int i = 0; i < getCharDArraySize(_env); ++i) {
		std::cout << i << " ";
		std::cout << _env[i] << std::endl;   
    }
	std::cout << "Path: " << this->_arg[0] << std::endl;
	int pipefd[2];
	if (pipe(pipefd) == -1)
		std::cout << "CGI pipe error" << std::endl;
	pid = fork();
	if (pid == 0){
		//child
		close(pipefd[0]);	// Close reading end
		dup2(pipefd[1], 1);	// send stdout to the pipe
		dup2(pipefd[1], 2);	// send stderr to the pipe
		close(pipefd[1]);
		execve(_arg[0], _arg, this->_env);
		exit(1);
	}
	else{
		//parent
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0){
			char buffer[1024];
            int bytes_read = read(pipefd[0], buffer, sizeof(buffer));
            if (bytes_read > 0)
            {
                // Null-terminate the buffer and print the output
                buffer[bytes_read] = '\0';
                std::cout << "Child process output: " << buffer << std::endl;
				response += buffer;
            }
		}
		else
            std::cerr << RED << "Child process failed to execute." << CLEAR << std::endl;
	}
	std::cout << "CGI Response: " << response << std::endl;
	delDArray(_env);
	delDArray(_arg);
	//return (response); //without header, etc
}


// to test cgi files, ./a.out <path to files>
int main(int argc, char **argv){
	std::map<std::string, std::string> content;
	cgi_handler cgi;
	cgi.execCGI(content, "sampleCGI/sampleCGI_c");
	//system("leaks a.out");
}