#if !defined(CONFIGURATIONLIST)
#define CONFIGURATIONLIST

#include <vector>
#include "../../includes/configuration/IConfiguration.hpp"
#include "../../includes/configuration/Defaults.hpp"
#include "../../includes/logger/ILogger.hpp"

class ConfigurationBlock;

class BlockList: public std::vector<IConfiguration *>
{
	private:
		ConfigurationBlock	*m_parent;
		ILogger				*m_logger;
	public:
		BlockList();
		~BlockList();
		void	setParent(ConfigurationBlock* block);
		void	setLogger(ILogger *logger);
		virtual	IConfiguration	*operator[](size_t index) const;
		virtual	IConfiguration	*at(size_t index);
		//todo: an iterator that returns the parent each time it is called.
};

#endif // CONFIGURATIONLIST
