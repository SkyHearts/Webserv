#include "server.hpp"

int parse_config( std::string config_file ) {
  std::string extension = ".conf";
  if (config_file.length() < extension.length() || config_file.compare(config_file.length() - extension.length(), extension.length(), extension)) {
    std::cerr << "Error: Invalid or missing .conf extension" << std::endl;
    return 1;
  }
  
  std::ifstream file(config_file.c_str());
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open configuration file" << std::endl;
  return 1;
}

int main() {
    Server server;

    if (ac == 1) {
		  std::cerr << "error: No configuration file specified\nUsage: ./webserv [file.conf]" << std::endl;
		  return 1;
	  }

	  std::string config_file = av[1];
	  if (parse_config(config_file))
		  return 1;

    // Add ports to listen on
    server.addPort(8080);
    server.addPort(8888);

    server.run();

    return 0;
}