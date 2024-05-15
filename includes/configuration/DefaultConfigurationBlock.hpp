#if !defined(DEFAULTCONFIGURATIONBLOCK_HPP)
#define DEFAULT_CONFIGURATION_HPP

#include "../../includes/configuration/IConfiguration.hpp"

class DefaultConfigurationBlock: public IConfiguration
{
private:
	/* data */
public:
	DefaultConfigurationBlock(/* args */);
	~DefaultConfigurationBlock();
};

DefaultConfigurationBlock::DefaultConfigurationBlock(/* args */)
{
}

DefaultConfigurationBlock::~DefaultConfigurationBlock()
{
}


#endif // DEFAULT_CONFIGURATION
