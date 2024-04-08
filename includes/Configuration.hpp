/*'Configuration': Verifies, reads, parses and stores the config 
file data. This data is used to initiate the 'Server' and the 'Router'*/

#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

#include <iostream>
#include <unordered_map>
#include <list>

class	Configuration {
	private:
		std::unordered_map<std::string, std::string> *fields;
		std::list<Configuration> sub_configurations;
	public:
		Configuration(std::unordered_map<std::string, std::string> *fields);
		Configuration(std::unordered_map<std::string, std::string> *fields, std::list<Configuration> *sub_conf);
		~Configuration();
		std::string&	operator[](const std::string field);
};

#endif
