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
    // Extract the boundary from the Content-Type header
    std::string contentTypeHeader = "Content-Type: multipart/form-data; ";
    size_t boundaryPos = requestBody.find(contentTypeHeader);
    if (boundaryPos == std::string::npos) {
        std::cerr << "No Content-Type header found or not multipart/form-data." << std::endl;
        return;
    }

    boundaryPos += contentTypeHeader.length();
    size_t boundaryEnd = requestBody.find("\r\n", boundaryPos);
    if (boundaryEnd == std::string::npos) {
        std::cerr << "Invalid Content-Type header format." << std::endl;
        return;
    }

    std::string boundary = requestBody.substr(boundaryPos, boundaryEnd - boundaryPos);
	std::cout << boundary << std::endl;

    // Split the request body into parts using the boundary
    std::vector<std::string> parts;
    size_t start = requestBody.find("--" + boundary);
    while (start != std::string::npos) {
        size_t end = requestBody.find("--" + boundary, start + boundary.length() + 2);
        if (end == std::string::npos) {
            break;  // End of parts
        }
        parts.push_back(requestBody.substr(start, end - start));
        start = end;
    }

    // Process each part (file or field)
    for (size_t i = 0; i < parts.size(); ++i) {
        const std::string& partData = parts[i];

        // Check if it's a file part (contains "filename" in Content-Disposition)
        if (partData.find("filename") != std::string::npos) {
            // Parse headers (Content-Disposition and Content-Type)
            std::string contentDisposition, contentType;
            std::istringstream partStream(partData);

            while (true) {
                std::string line;
                std::getline(partStream, line);
                if (line.empty()) {
                    break;  // End of headers
                } else if (line.find("Content-Disposition:") != std::string::npos) {
                    contentDisposition = line;
                } else if (line.find("Content-Type:") != std::string::npos) {
                    contentType = line;
                }
            }

            // Extract the file name from Content-Disposition
            size_t filenamePos = contentDisposition.find("filename=\"");
            if (filenamePos != std::string::npos) {
                filenamePos += 10;  // Skip "filename=\""
                size_t filenameEnd = contentDisposition.find("\"", filenamePos);
                if (filenameEnd != std::string::npos) {
                    std::string filename = contentDisposition.substr(filenamePos, filenameEnd - filenamePos);
                    std::cout << "Received file: " << filename << std::endl;

                    // Process the file content (e.g., save it to a file)
                    std::ofstream outputFile(filename.c_str(), std::ios::binary);
                    outputFile << partStream.rdbuf();
                    outputFile.close();
                    std::cout << "Saved file: " << filename << std::endl;
                }
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