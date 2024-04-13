#include "../includes/Configuration.hpp"

/*'Configuration': Verifies, reads, parses and stores the config 
file data. This data is used to initiate the 'Server' and the 'Router'*/

Configuration::Configuration(std::unordered_map<std::string, std::string> *fields, std::list<Configuration> *sub_conf)
{
    this->fields = fields;
    this->sub_configurations = *sub_conf;
}

Configuration::Configuration(std::unordered_map<std::string, std::string> *fields)
{
    this->fields = fields;
}

Configuration::~Configuration()
{
    delete this->fields;
}