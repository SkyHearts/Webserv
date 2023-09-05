# WEBSERV
The aim of this project is to create a simple HTTP server.

Contributors to this project are: 
* [Nur Almas | nnorazma](https://github.com/M4rrs)
* [Wong Hong You | hwong](https://github.com/thewongwaae)
* [Yim Jiun Jye | jyim](https://github.com/SkyHearts)

## Current Workflow
Technically non-existent. **But,**
* **Hong You:** [Main server handling.](#hong-you)
* **Me:** [Handling requests and responses.](#mars)
* **JJ:** [Configuration and CGI](#jj)

## Hong You

## Mars
I have no clue what I'm doing. But here's the plan — sort of:
* From the server loop, the request string is passed into parseResponse() of the request class.
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

## JJ

## References
[Web Server Concepts and Examples](https://www.youtube.com/watch?v=9J1nJOivdyw)
[C++ Web Server from Scratch](https://youtu.be/YwHErWJIh6Y)
[cclaude's 42 Webserv Github Page](https://github.com/cclaude42/webserv) **Disclaimer: Do NOT copy another persons work. Cclaude's page includes a very detailed Readme page with very helpful resources that we used as references.**
[HTTP related RFCs](https://httpwg.org/specs/)