#if !defined(LOCATIONBLOCK_HPP)
#define LOCATIONBLOCK_HPP
#include "logger/ILogger.hpp"
#include "response/IURIMatcher.hpp"
#include "configuration/ConfigurationBlock.hpp"

/*
A Special class that takes in a IURIMatcher and provides a method for testing if an URI matches
the configuration block.
*/

class LocationBlock : public ConfigurationBlock
{
	private:
		bool	m_is_regex;
	public:
		LocationBlock(ILogger &logger, const std::string name, bool is_regex);
		LocationBlock(const ConfigurationBlock &parent, const std::string name, bool is_regex);
		~LocationBlock();
		virtual bool	isRegex(void) const;
};

#endif // LOCATIONBLOCK_HPP
