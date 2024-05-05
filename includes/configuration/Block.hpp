#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "IBlock.hpp"
#include <stdexcept>
#include <map>
#include "logger/ILogger.hpp"

class Block : public IBlock
{
	private:
		std::map<std::string, std::vector<IBlock *> > _blocks;
		std::map<std::string, std::vector<std::string> > _stringVectors;
		std::map<std::string, std::string> _strings;
		std::map<std::string, int> _ints;
		std::map<std::string, size_t> _size_ts;
		std::map<std::string, bool> _bools;
		std::map<std::string, std::vector<std::string>* > _directives;
		ILogger&	_logger;
		const std::string	_name;

	public:
		Block(ILogger &logger, const std::string name);
		Block(const Block &parent, const std::string name);
		~Block();

		virtual const std::vector<IBlock *>&	getBlocks(const std::string &key) const;
		virtual const std::vector<std::string>&	getStringVector(const std::string &key) const;
		virtual const std::string getString(const std::string &parameter) const;
		virtual int		getInt(const std::string &parameter, size_t index) const;
		virtual size_t	getSize_t(const std::string &parameter, size_t index) const;
		virtual bool	getBool(const std::string &parameter, size_t index) const;
		virtual	void	addBlock(const std::string& name, IBlock *block);
		virtual	void	addDirective(const std::string& name, std::vector<std::string> *parameters);
		void			print(size_t depth) const;
		virtual const std::string&	getName() const;
};

#endif // BLOCK_HPP
// Path: includes/configuration/Block.hpp
