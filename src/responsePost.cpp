#include "responsePost.hpp"

/*============================================================================*/
ResponsePost::ResponsePost( void ) : ResponseBase() { }

ResponsePost::ResponsePost( std::string filePath, std::map < std::string, std::string > reqHead, std::string reqBody, size_t payload, ServerConfig portinfo) : ResponseBase() {
	clearResources();

	this->_portinfo = portinfo;
	this->_path.append(filePath);
	this->_requestHeader = reqHead;
	this->_requestBody = reqBody;
	this->_payload = payload;

	std::cout << RED << "path to dir: " << _path << CLEAR << std::endl;
	if (validateResource(this->_portinfo.root + this->_path)) {
		std::cout << RED << "valid dir?" << CLEAR << std::endl;
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

bool ResponsePost::validateResource( const std::string &name ) {
	struct stat sb;

	if (stat(name.c_str(), &sb) == 0) {
		if (S_ISDIR(sb.st_mode))
		{	std::cout << RED << "valid dir?" << CLEAR << std::endl;
			return true;}
		else if (S_ISREG(sb.st_mode))
		{	std::cout << RED << "valid file?" << CLEAR << std::endl;
			return true;}
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

	std::cout << RED << "path before append: " << this->_path << CLEAR << std::endl;
	if (isUpload) {
		this->_path.insert(0, this->_portinfo.root);
		this->_path.append("/" + std::to_string(this->_statusCode) + ".html");
	}
	this->_file.open(this->_path);

	if (!this->_file.is_open())
		setStatusCode(500);
}

static std::string decodeEncoding( std::string &input ) {
	std::string decoded;
	size_t input_len = input.length();
	size_t pos = 0;
	int ascii;

	while (pos < input_len) {
		if (input[pos] == '%' && (pos + 2) < input_len) {
			char hex[3] = { input[pos + 1], input[pos + 2], 0 };

			if (sscanf(hex, "%x", &ascii) == 1) {
				decoded += static_cast<char>(ascii);
				pos += 3;
			}
			else
				decoded += input[pos++];
		}
		else
			decoded += input[pos++];
	}

	return decoded;
}

/*
	Handler for content type of application/x-www-urlencoded
*/
void ResponsePost::handleTextData( std::string requestBody ) {
	std::string key, value;
	std::string data = decodeEncoding(requestBody);
	std::replace(data.begin(), data.end(), '+', ' ');
	size_t equal = data.find('=');

	if (equal != std::string::npos) {
		key = data.substr(0, equal);
		value = data.substr(equal + 1);

		std::ofstream file(_portinfo.root + "/" + key + ".txt");
		if (!file.is_open()) { 
			setStatusCodePost(500, 0);
			return ;
		}
		file << value;
		if (file.bad()) {
			std::cout << RED << "file bad?" << CLEAR << std::endl;
			setStatusCodePost(500, 0);
		}
		else 
			setStatusCodePost(201, 1);
		file.close();
	}
	else {
		setStatusCodePost(204, 1);
	}
}

/*
	Planning to split up the saving data section to minimise repeated code for ^ and v.
	Perhaps pass in address of file and value, and return status code?
*/
void ResponsePost::handleMultipartFormData( std::string filename, std::string rawData ) {

	if (validateResource("/" + filename)) {
		setStatusCodePost(409, 1);
	}
	else {
		std::ofstream file(_portinfo.root + "/uploads/" + filename);
		if (!file.is_open()) { setStatusCodePost(500, 0); }
		file << rawData;
		if (file.bad()) { setStatusCodePost(500, 0); }
		else { setStatusCodePost(201, 1); }
		file.close();
	}
}

/*
	1) Search for POST content-type
	2) if text, handle the text
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

	// what happens if it happens to not be any of these content types?
}

void ResponsePost::generateResponse( void ) {

	std::cout << RED << "path to response page: " << _path << CLEAR << std::endl;
	if (this->_statusCode == 500)
		_response.append(generateResponseISE());
	else {
		this->_response.append("HTTP/1.1 " + std::to_string(this->_statusCode) + " " + this->_statusCodes[this->_statusCode] + "\r\n");
		this->_response.append("Content-Type: " + this->_contentTypes["html"] + "\r\n\r\n");
		std::string line;
		while (getline(this->_file, line))
			this->_response.append(line);
	}
}