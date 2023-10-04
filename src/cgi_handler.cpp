/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyim <jyim@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:08:33 by jyim              #+#    #+#             */
/*   Updated: 2023/09/27 13:41:00 by jyim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi_handler.hpp"

int getCharDArraySize( char** array ) {
	int i = 0;

	while (array[i] != NULL)
		i++;
	return (i);
}

cgi_handler::cgi_handler( void ) : _env(NULL), _arg(NULL) { }

cgi_handler::~cgi_handler( void ) { }

void cgi_handler::reassginDArray( char **dest, char **src ) {
	int srcSize = getCharDArraySize(src);

	for (int i = 0; i < srcSize; ++i)
		dest[i] = src[i];

	delete[] src;
}

void cgi_handler::delDArray( char **dArray ) {
	for (int i = 0; i < getCharDArraySize(dArray); ++i)
		delete[] dArray[i];   
    delete[] dArray;
}

void cgi_handler::addEnv( std::string envVar ) {
	int envSize;
	char** tmp_env;

	if (_env == NULL) {
		_env = new char*[2];
		_env[0] = new char[envVar.size() + 1];
		std::strcpy(_env[0], envVar.c_str());
		_env[1] = NULL;
	}
	else {
		envSize = getCharDArraySize(_env);
		tmp_env = new char*[envSize + 2];
		reassginDArray(tmp_env, _env);
		tmp_env[envSize] = new char[envVar.size() + 1];
		std::strcpy(tmp_env[envSize], envVar.c_str());
		tmp_env[envSize + 1] = NULL;
		_env = tmp_env;
	}
}

void cgi_handler::addCharEnvs( char** payload ) {
	int payLoadSize = getCharDArraySize(payload);
	int envSize;
	char** tmp_env;

	if (_env == NULL) {
		_env = new char*[payLoadSize + 1];

		for (int i = 0; i < payLoadSize; ++i) {
			_env[i] = new char[strlen(payload[i] + 1)];
			std::strcpy(_env[i], payload[i]);
		}
		_env[payLoadSize] = NULL;
	}
	else {
		envSize = getCharDArraySize(_env);
		tmp_env = new char*[envSize + payLoadSize + 1];
		reassginDArray(tmp_env, _env);

		for (int i = 0; i < payLoadSize; ++i) {
			tmp_env[envSize + i] = new char[strlen(payload[i]) + 1];
			std::strcpy(tmp_env[envSize + i], payload[i]);
		}
		tmp_env[envSize + payLoadSize] = NULL;
		_env = tmp_env;
	}
}

/*
    if any of map content does not exist, ENV=<EMPTY>
*/
void cgi_handler::createEnv( std::map<std::string, std::string> content, char** payload, ServerConfig portInfo ) {
	addEnv("SERVER_SOFTWARE=WebServ");
	addEnv("SERVER_PORT=" + std::to_string(portInfo.listen));
	addEnv("REQUEST_METHOD=" + content["Method"]);
	addEnv("PATH_INFO=" + content["Path"]);
	addEnv("PATH_TRANSLATED=" + portInfo.root + content["Path"]);
	addEnv("HTTP_REFERER=" + content["Referer"]);
	addEnv("HTTP_ACCEPT="+ content["Accept"]) ;

    if (!payload)
		addCharEnvs(payload);
}

void cgi_handler::addArg( std::string arg ) {
	int argSize;
	char** tmp_arg;

	if 	(_arg == NULL) {
		_arg = new char*[2];
		_arg[0] = new char[arg.size() + 1];
		std::strcpy(_arg[0], arg.c_str());
		_arg[1] = NULL;
	}
	else {
		argSize = getCharDArraySize	(_arg);
		tmp_arg = new char*[argSize + 2];
		reassginDArray(tmp_arg, _arg);
		tmp_arg[argSize] = new char[arg.size() + 1];
		std::strcpy(tmp_arg[argSize], arg.c_str());
		tmp_arg[argSize + 1] = NULL;
		_arg = tmp_arg;
	}
}

void cgi_handler::createArg( std::string path ) {
	addArg(path);
}


void cgi_handler::execCGI( std::map<std::string, std::string> content, std::string path, ServerConfig portInfo, char** payload ) {
	pid_t pid;
	std::string response;

	createEnv(content, payload, portInfo);
	createArg(path);

	std::cout << std::endl;
	std::cout << "Path: " << this->_arg[0] << std::endl;
	int pipefd[2];
	if (pipe(pipefd) == -1)
		std::cout << "CGI pipe error" << std::endl;
	pid = fork();
	if (pid == 0) {
		//child
		close(pipefd[0]);	// Close reading end
		dup2(pipefd[1], 1);	// send stdout to the pipe
		dup2(pipefd[1], 2);	// send stderr to the pipe
		close(pipefd[1]);
		execve(_arg[0], _arg, this->_env);
		exit(1);
	}
	// else{
	// 	//parent
	// 	int status;
	// 	waitpid(pid, &status, 0);
	// 	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
	// 		char buffer[1024];
    //         int bytes_read = read(pipefd[0], buffer, sizeof(buffer));
    //         if (bytes_read > 0)
    //         {
    //             // Null-terminate the buffer and print the output
    //             buffer[bytes_read] = '\0';
    //             std::cout << "Child process output: " << buffer << std::endl;
	// 			response += buffer;
    //         }
	// 	}
	// 	else
    //         std::cerr << RED << "Child process failed to execute." << CLEAR << std::endl;
	// }
	std::cout << "CGI Response: " << response << std::endl;
	delDArray(_env);
	delDArray(_arg);
	//return (response); //without header, etc
}


// to test cgi files, ./a.out <path to files>
int main(int argc, char **argv){
	std::map<std::string, std::string> content;
	cgi_handler cgi;
    ServerConfig portinfo;
    portinfo.root = ROOT_DEFAULT;
    portinfo.listen = 8080;
	const char *sample_payload[3] = {"payload1=hello", "payload2=world!", NULL};
    content["Path"] = "/cgi-bin/printEnv";
    content["Method"] = "GET";
    content["Referer"] = "localhost:8080";
    content["Accept"] = "text/html";
	// for(int i = 0; i < 3 != NULL; ++i) {
	// 	std::cout << i << " ";
	// 	std::cout << sample_payload[i] << std::endl;   
    // }
	// for(int i = 0; sample_payload[i] != NULL; ++i) {
	// 	std::cout << i << " ";
	// 	std::cout << sample_payload[i] << std::endl;   
    // }
	cgi.execCGI(content, "../html/cgi-bin/printEnv", portinfo, const_cast<char **>(sample_payload));
	// cgi.execCGI(content, "sampleCGI/sampleCGI_c");
	//system("leaks a.out");
}