# WEBSERV
The aim of this project is to create a simple HTTP server.

Contributors to this project are: 
* [Nur Almas | nnorazma](https://github.com/M4rrs)
* [Wong Hong You | hwong](https://github.com/thewongwaae)
* [Yim Jiun Jye | jyim](https://github.com/SkyHearts)

## Current Workflow
Technically non-existent. **But,**
* **JJ:** [Configuration file parsing and CGI](#jj)
* **Hong You:** [Main server handling.](#hong-you)
* **Me:** [Handling requests and responses.](#mars)

## JJ

<b>Configuration</b>
Taking some referernce from NGINX configuration format, parse information necessary to create a minimum viable webserver. Information here will be required to run the server and send response back to browser.
* Reading from .conf file
* Scan word by word for keywords to be further process
* Keyword/format supported are as follows
```
server 			//server keyword to parse a server block
{ 				//Start of a server block
	listen 80 81 82 83; 	//listen keyword to parse first non occupied port
	root html;				//root keyword to parse the path to root folder of webserver
	index index.html;		//index keyword to parse the index page of server
	server_name trinity.com;	//server_name keyword to parse the name of website
	
	error_pages 404 html/404.html; //error_page keyword to parse error number and path to error page html
	error_pages 405 html/405.html;

	location /cgi-bin { //location keyword to parse URI of website
		allowedMethods   GET POST; //allowedMethods keyword to parse the permitted method of a request
		index select_cgi.html; //index keyword to parse the index page of location URI, root + URI + index
	}
	
	location /directory {
		allowedMethods   GET;
		autoindex on;
	}
}		//End of a server block
//Any keyword not supported will result in exception being thrown
```

<b>CGI Execution</b>
A simple CGI execution function is also written to run scripts/executable in child process and response will be return.
* https://handoutset.com/wp-content/uploads/2022/05/Web-Server-Programming-Neil-Gray.pdf
* Environtment variable(Env) of webserver and argument(Arg) will be created
* Env and Arg will be passed to execeve() to run scripts or executable


## Hong You
The part I worked on deals with the main handler loop of the server. This covers the following:

<b>Initialisation</b>
* Creating the socket
* Binding the socket to the port
* Listening for incoming connections
* Accepting connections
* Closing connections

<b>Connections</b>
* Reading from the socket(s)
* Writing to the socket(s)
* Closing connections with the socket(s)

## Mars
My job is to handle request parsing, method handling and responses. Here's what I deal with:

* Dividing the request into the header and body
* For all methods I must validate the resource path and validate method permissions.
* **GET** simply covers locating the required resource, opening and reading the file, and then returning the contents as the response.
* **POST**:
	* The types of data sent through POST is limited to ```application/x-www-form-urlencoded``` and ```multipart/form-data```
	* The data to be stored must be validated; if it exists, send a 409 Conflict.
	* Else, store the data and return a 201 Created.
	* For ```application/x-www-form-urlencoded```:
		* The data sent through will be encoded.
		* Therefore in order to store the data completely, it has to be decoded.
* **DELETE**:
	* We chose to only test DELETE from command line. (To save time)
	* The data stored is validated, deleted, and return a response 200 OK :thumbs_up:

## References
[Web Server Concepts and Examples](https://www.youtube.com/watch?v=9J1nJOivdyw)

[C++ Web Server from Scratch](https://youtu.be/YwHErWJIh6Y)

[cclaude's 42 Webserv Github Page](https://github.com/cclaude42/webserv)

**Disclaimer: Please don't copy another persons work. Cclaude's page includes a very detailed Readme page with very helpful resources that we used as references.**

[HTTP related RFCs](https://httpwg.org/specs/)