#include "responsePost.hpp"
#include "cgi_handler.hpp"

/*============================================================================*/
ResponsePost::ResponsePost( void ) : ResponseBase() { }

ResponsePost::ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, size_t payload, ServerConfig portinfo) : ResponseBase() {
	clearResources();

	this->_portinfo = portinfo;
	this->_path.append(filePath);
	this->_requestHeader = reqHead;
	this->_requestBody = reqBody;
	this->_payload = payload;

	if (validateResource(this->_portinfo.root + this->_path)) {
		if (checkPermissions("POST"))
			saveData();
		else
			setStatusCodePost(405, 0);
	}
	else
		setStatusCodePost(404, 0);

	generateResponse();
}

ResponsePost::~ResponsePost( void ) { }
/*============================================================================*/

/*
	Clear up necessary resources to be used next call
*/
void ResponsePost::clearResources( void ) {
	this->_fileName.clear();
	this->_requestHeader.clear();
	this->_requestBody.clear();
}

/*
	Checks if the resource is a directory or file
*/
bool ResponsePost::validateResource( const std::string &name ) {
	struct stat sb;

	if (stat(name.c_str(), &sb) == 0) {
		if (S_ISDIR(sb.st_mode))
			return true;
		else if (S_ISREG(sb.st_mode))
			return true;
	}
	
	return false;
}

/*
	Sets the status code for the response.
	If status is upload related (201, 409, etc), append correct directory to path.
	Else, open error page.
	If the file fails to open, set status 500.
*/
void ResponsePost::setStatusCodePost( int status, int isUpload ) {
	setStatusCode(status);
	setContentType("html");

	this->_path.clear();
	this->_path.append(this->_portinfo.root);
	if (isUpload)
		this->_path.append("/upload");
	this->_path.append("/" + std::to_string(this->_statusCode) + ".html");

	std::cout << RED << "status code path: " << this->_path << CLEAR << std::endl;
	this->_file.open(this->_path);
	if (!this->_file.is_open())
		setStatusCode(500);
}

/*
	Handler for content type of application/x-www-urlencoded
	Split incoming data into left of = and right of =
	Use left as Key and filename, then right as data

	If file of same name already exists, overwrite data
*/
void ResponsePost::handleTextData( std::string requestBody ) {
	std::string key, value;
	std::string data = decodeEncoding(requestBody);
	std::replace(data.begin(), data.end(), '+', ' ');
	size_t equal = data.find('=');

	if (equal != std::string::npos) {
		key = data.substr(0, equal);
		value = data.substr(equal + 1);

		std::ofstream file(_portinfo.root + "/uploads/" + key + ".txt");
		if (!file.is_open())
			return setStatusCode(500);

		file << value;
		if (file.bad())
			setStatusCode(500);
		else 
			setStatusCodePost(201, 1);

		file.close();
	}
	else
		setStatusCodePost(204, 1);
}

/*
	Specifically to throw client input from calculator to CGI
*/
void ResponsePost::handleCalc( std::string requestBody ) {
	size_t expression_pos = requestBody.find("EXPR=");
	cgi_handler cgi;

	_usingCGI = true;
	_response.clear();
	if (expression_pos != std::string::npos) {
		std::string expression = requestBody.substr(expression_pos);
		const char *payload[2] = {expression.c_str(), NULL};
		_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
		_response += cgi.execCGI(this->_requestHeader, _portinfo.root + "/calc/eval", this->_portinfo, const_cast<char **>(payload));
	}
	else
		_response = "Error";
}

/*
	Create file of given filename (including extension)

	If file of same name already exists, set status code to 409
	Else, write rawData to file

	If file fails to open, set status code to 500
*/
void ResponsePost::handleMultipartFormData( std::string filename, std::string rawData ) {
	filename.insert(0, _portinfo.root + "/uploads/");
	filename = decodeEncoding(filename);

	if (validateResource(filename))
		setStatusCodePost(409, 1);
	else {
		std::ofstream file(filename);
		if (!file.is_open()) {
			setStatusCode(500); 
			return;
		}

		file << rawData;
		if (file.bad())
			setStatusCodePost(500, 0);
		else
			setStatusCodePost(201, 1);

		file.close();
	}
}

/*
	1) Search for POST content-type
	2) if urlencoded input, handle and save the text
	3) If plaintext, we know its from calculator CGI call
	3) if it contains form data;
		- search for boundary
		- split form header and body
		- save the body into rawData (not decoded)
		- handle multipart/form-data
*/
void ResponsePost::saveData( void ) {
	std::string contentType = _requestHeader["Content-Type"];

	if (contentType.find("application/x-www-form-urlencoded") != std::string::npos) {
		handleTextData(_requestBody);
		return ;
	}
	else if (contentType.find("text/plain") != std::string::npos) {
		handleCalc(_requestBody);
		return ;
	}

	_boundary = contentType.substr(contentType.find("boundary=") + 9);

	std::istringstream formBody(_requestBody);
	std::string line, key, value;
	std::vector< std::string > formHead;

	std::getline(formBody, line);
	while (std::getline(formBody, line, '\n') && line != "\r")
		formHead.push_back(line);
	std::string filename = formHead[0].substr(formHead[0].find("filename=") + 10);
	filename = filename.substr(0, filename.find("\""));

	std::ostringstream rawData;
	std::string rawDataStr;
	while (std::getline(formBody, line, '\n')) {
		if (line == "--" + _boundary + "--")
			break ;
		rawData << line << "\n";
	}
	rawDataStr = rawData.str();
	size_t lastNL = rawDataStr.find_last_of("\n");
	if (lastNL != std::string::npos)
		rawDataStr = rawDataStr.substr(0, lastNL);
	size_t lastNL2 = rawDataStr.find_last_of("\n");
	if (lastNL2 != std::string::npos)
		rawDataStr = rawDataStr.substr(0, lastNL2);

	if (contentType.find("multipart/form-data") != std::string::npos)
		handleMultipartFormData(filename, rawDataStr);
}

/*
	Generate response based on status code
	- 500: Internal Server Error
	- 201: Created
	- 204: No Content
	- 409: Conflict
	- 405: Method Not Allowed
	- 404: Not Found
*/
void ResponsePost::generateResponse( void ) {
	if (this->_statusCode == 500)
		_response.append(generateResponseISE());
	else if (_usingCGI)
		return ;
	else {
		this->_response.append("HTTP/1.1 " + std::to_string(this->_statusCode) + " " + this->_statusCodes[this->_statusCode] + "\r\n");
		this->_response.append("Content-Type: " + this->_contentTypes[this->_contentType] + "\r\n\r\n");
		std::string line;
		while (getline(this->_file, line))
			this->_response.append(line);
	}
}