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
I have no clue what I'm doing. But here's the plan — sort of:
* From the server loop, the request string is passed into ```parseResponse()``` of the request class.
* The request content is split into whatever variables I felt were necessary at the moment.
* Validate path and set status code accordingly.
* Read and append details from respective html files.
* Response returned to the server class.
<br>

* For now I am only handling GET requests.
* I don't know how to deal with POST.
* I don't know how to deal with DELETE either.
<br>

* I decided to split the response handling to the respective methods, with each individual class. Still a WIP.

## References
[Web Server Concepts and Examples](https://www.youtube.com/watch?v=9J1nJOivdyw)

[C++ Web Server from Scratch](https://youtu.be/YwHErWJIh6Y)

[cclaude's 42 Webserv Github Page](https://github.com/cclaude42/webserv)

**Disclaimer: Please don't copy another persons work. Cclaude's page includes a very detailed Readme page with very helpful resources that we used as references.**

[HTTP related RFCs](https://httpwg.org/specs/)