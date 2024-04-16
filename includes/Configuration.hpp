/*'Configuration': Verifies, reads, parses and stores the config 
file data. This data is used to initiate the 'Server' and the 'Router'*/

#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

#include "IConfiguration.hpp"
#include <iostream>
#include <map>
#include <list>

class	Configuration : public IConfiguration{
	private:
		std::map<std::string, std::string> *fields;
		std::list<Configuration> sub_configurations;
	public:
		Configuration(std::map<std::string, std::string> *fields);
		Configuration(std::map<std::string, std::string> *fields, std::list<Configuration> *sub_conf);
		~Configuration();
		std::string&	operator[](const std::string field);

		// temporary getters
		virtual size_t getClientHeaderBufferSize() const{ return 0; };
		virtual size_t getClientBodyBufferSize() const{ return 0; };
		virtual size_t getClientMaxBodySize() const{ return 0; };
		virtual size_t getClientMaxUriSize() const{ return 0; };
		virtual size_t getLogBufferSize() const{ return 0; };
		virtual size_t getMaxConnections() const{ return 0;}
		virtual int getPort() const{ return 0;}
		virtual LogLevel getLogLevel() const{ return INFO;};
		virtual bool getErrorLogEnabled() const{ return true;};
		virtual bool getAccessLogEnabled() const{ return true;};
		virtual std::string getErrorLogFile() const{ return "";};
		virtual std::string getAccessLogFile() const{ return "";};
		virtual std::set<std::string> getSupportedMethods() const{ std::set<std::string> s; return s; };
};

#endif
