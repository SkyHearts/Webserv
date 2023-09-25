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

	if (checkPermissions("POST"))
		saveData();
	generateResponse();
}

ResponsePost::~ResponsePost( void ) { }
/*============================================================================*/

void ResponsePost::clearResources( void ) {
	this->_fileName.clear();
	this->_requestHeader.clear();
	this->_requestBody.clear();
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
void ResponsePost::handleTextData( std::string &requestBody ) {
	std::string key, value;
	std::string data = decodeEncoding(requestBody);
	std::replace(data.begin(), data.end(), '+', ' ');
	size_t equal = data.find('=');

	if (equal != std::string::npos) {
		key = data.substr(0, equal);
		value = data.substr(equal + 1);

		std::ofstream file(_portinfo.root + "/" + key + ".txt");
		file << value;
		file.close();
	}
}

void ResponsePost::handleMultipartFormData( std::string &requestBody ) {
	std::cout << "Boundary is " << _boundary << std::endl;
	std::string boundary = _boundary.substr(2);

	// Find the start of the first part
	size_t start = requestBody.find("--" + boundary);
	if (start != std::string::npos) {
		start += boundary.length() + 2; // Skip "--" and boundary
		size_t end = requestBody.find("--" + boundary, start);
		if (end != std::string::npos) {
			std::string partData = requestBody.substr(start, end - start);

			// Process the part data (file or field)
			std::istringstream partStream(partData);

			// Check if it's a file part (contains "filename" in Content-Disposition)
			if (partData.find("filename") != std::string::npos) {
				// Parse headers (Content-Disposition and Content-Type)
				std::string contentDisposition, contentType;
				while (true) {
					std::string line;
					std::getline(partStream, line);
					if (line.empty()) {
						break; // End of headers
					} else if (line.find("Content-Disposition:") != std::string::npos) {
						contentDisposition = line;
					} else if (line.find("Content-Type:") != std::string::npos) {
						contentType = line;
					}
				}

				// Extract the file name from Content-Disposition
				size_t filenamePos = contentDisposition.find("filename=\"");
				if (filenamePos != std::string::npos) {
					filenamePos += 10; // Skip "filename=\""
					size_t filenameEnd = contentDisposition.find("\"", filenamePos);
					if (filenameEnd != std::string::npos) {
						std::string filename = contentDisposition.substr(filenamePos, filenameEnd - filenamePos);
						std::cout << "Received file: " << filename << std::endl;

						// Process the file content (e.g., save it to a file)
						std::ofstream outputFile(filename, std::ios::binary);
						while (partStream) {
							char buffer[4096];
							partStream.read(buffer, 4096);
							outputFile.write(buffer, partStream.gcount());
						}
						outputFile.close();
						std::cout << "Saved file: " << filename << std::endl;
					}
				}
			} else {
				// It's a regular form field (not a file)
				std::string fieldName;
				while (true) {
					std::string line;
					std::getline(partStream, line);
					if (line.empty()) {
						break; // End of headers
					} else if (line.find("name=") != std::string::npos) {
						// Extract the field name from Content-Disposition
						size_t namePos = line.find("name=\"");
						if (namePos != std::string::npos) {
							namePos += 6; // Skip "name=\""
							size_t nameEnd = line.find("\"", namePos);
							if (nameEnd != std::string::npos) {
								fieldName = line.substr(namePos, nameEnd - namePos);
							}
						}
					}
				}

				// Extract and print the field value
				std::string fieldValue;
				std::getline(partStream, fieldValue);
				std::cout << "Received form field: " << fieldName << " = " << fieldValue << std::endl;
			}
		}
	}
}

void ResponsePost::saveData( void ) {
	std::string search = _requestHeader["Content-Type"];
	this->_boundary = search.substr(search.find("boundary=") + 1);

	std::istringstream formBody(this->_requestBody);
	std::string line, key, value;
	std::map< std::string, std::string > formHead;

	std::getline(formBody, line);

	while (getline(formBody, line) && !line.empty()) {
		try {
			int colon = line.find(':');
			key = line.substr(0, colon);
			value = line.substr(colon + 2);
			formHead.insert(std::pair< std::string, std::string >(key, value));
		}
		catch (std::exception const &e) {
			break;
		}
	}

	std::string contentType = _requestHeader["Content-Type"];
	std::cout << "Content is a " << contentType << std::endl;

	if (contentType.find("application/x-www-form-urlencoded") != std::string::npos) {
		handleTextData(_requestBody);
	}
	else if (contentType.find("multipart/form-data") != std::string::npos) {
		std::cout << "Is a file input" << std::endl;
		handleMultipartFormData(_requestBody);
	}
}

void ResponsePost::generateResponse( void ) {
	_response.append("bla");
}