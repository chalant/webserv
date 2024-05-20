#include "../../includes/configuration/LocationBlock.hpp"
#include "../../includes/configuration/ConfigurationBlock.hpp"

LocationBlock::LocationBlock(ILogger &logger, const std::string name,
                             Defaults &defaults, bool is_regex)
    : ConfigurationBlock(logger, name, defaults), m_is_regex(is_regex)
{
}

LocationBlock::LocationBlock(const ConfigurationBlock &parent,
                             const std::string name, Defaults &defaults,
                             bool is_regex)
    : ConfigurationBlock(parent, name, defaults), m_is_regex(is_regex)
{
}

LocationBlock::~LocationBlock() {}

bool LocationBlock::isRegex() const { return m_is_regex; }
