#include "server.hpp"
#include "parse.hpp"
#include <fstream>

int parse_config( std::string config_file ) {
	std::string extension = ".conf";
	if (config_file.length() < extension.length() || config_file.compare(config_file.length() - extension.length(), extension.length(), extension)) {
		std::cerr << RED << "Error: Invalid or missing .conf extension" << CLEAR << std::endl;
		return 1;
	}
	
	std::ifstream file(config_file.c_str());
	if (!file.is_open()) {
		std::cerr << RED << "Error: Unable to open configuration file" << CLEAR << std::endl;
		return 1;
	}

	return 0;
}

int main( int ac, char **av ) {

	if (ac < 0 || ac > 2) {
		std::cerr << RED << "Error: Invalid usage\nUsage: ./webserv [file.conf]" << CLEAR << std::endl;
		return 1;
	}
	std::string config_file = "server.conf";
	if (ac == 2)
		config_file = av[1];
	if (parse_config(config_file))
		return 1;

	Server server;
	// Add ports to listen on
	try
	{
		Config S(av[1]);
		std::vector<ServerConfig> ports = S.get_servers();
		std::vector<ServerConfig>::iterator iter;
		for(iter = ports.begin(); iter < ports.end(); iter++)
		{
			server.addPort((*iter).listen);
		}
		server.run();
	}
	catch (std::invalid_argument& e)
	{
        std::cerr << "Config error:" << e.what() << std::endl;
        return -1;
    }
	return 0;
}


/* Test config class, prints out all information parse by config class */
//int main(int argc, char **argv){
//	(void)argc;
//	try // or write in parseServerBlock
//	{
//		Config S(argv[1]);
//		std::cout << std::endl << std::endl << std::endl;
		
//		std::vector<ServerConfig> ports = S.get_servers();
//		std::vector<ServerConfig>::iterator iter;
//		for(iter = ports.begin(); iter < ports.end(); iter++)
//		{
//			std::cout << "Server {" << std::endl;
//			std::cout << iter->name << std::endl;
//			std::cout << "Listen: " << (*iter).listen << std::endl;
//			std::cout << "Root: " << (*iter).root << std::endl;
//			std::cout << "Index: " << (*iter).index << std::endl;
//			if(!(*iter).errorPages.empty()){
//				std::cout << "KEY\tELEMENT\n";
//				for (std::map<int, std::string>::iterator itr = (*iter).errorPages.begin(); itr != (*iter).errorPages.end(); ++itr) {
//					std::cout << itr->first << '\t' << itr->second << '\n';
//				}
//			}
//			std::cout << std::endl;
//			if (!(*iter).locations.empty())
//			{
//				for (std::vector<Location>::iterator loc = (*iter).locations.begin(); loc < (*iter).locations.end(); loc++)
//				{
//					if (!(*loc).uri.empty())
//						std::cout << "location uri: " << (*loc).uri << std::endl;
//					if (!(*loc).index.empty())
//						std::cout << "location index: " << (*loc).index << std::endl;
//					std::cout << "location autoindex: " << (*loc).autoindex << std::endl;
//					if (!(*loc).allowedMethods.empty()){
//						std::cout << "allowedMethods: ";
//						for (std::vector<std::string>::iterator allow = (*loc).allowedMethods.begin(); allow < (*loc).allowedMethods.end(); allow++)
//						{
//							std::cout << (*allow) << " ";
//						}
//						std::cout << std::endl;
//					}
//					std::cout << std::endl;
//				}
//			}
//			std::cout << "}" << std::endl;
//		}
//		std::cout << std::endl;
//		//if (!S.get_servers().empty())
//		//	 ;//free servers
//	}
//	catch (std::invalid_argument& e)
//	{
//        std::cerr << "Config error:" << e.what() << std::endl;
//			//write free here?
//        return -1;
//		//exit(1);
//    }
//}