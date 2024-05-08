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
		IURIMatcher*	m_uri_matcher;
	public:
		LocationBlock(ILogger &logger, const std::string name, IURIMatcher* uri_matcher);
		LocationBlock(const ConfigurationBlock &parent, const std::string name, IURIMatcher* uri_matcher);
		~LocationBlock();
		virtual bool	matchURI(const std::string& uri);
};

#endif // LOCATIONBLOCK_HPP
