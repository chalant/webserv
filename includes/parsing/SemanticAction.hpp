#ifndef SEMANTICACTION_HPP
# define SEMANTICACTION_HPP

#include <vector>
#include "Tokenizer.hpp"

class ASemanticAction {
	public:
		ASemanticAction() {};
		//virtual ~ASemanticAction() = 0;
		virtual	void	execute(std::vector<std::string>& input, std::vector<std::string>& output) = 0;
};

//relays input to output.
class RelayAction: public ASemanticAction {
	public:
		RelayAction() {};
		~RelayAction() {};
		void	execute(std::vector<std::string>& input, std::vector<std::string>& output) {
			for (size_t i = 0; i < input.size(); i++) {
				output.push_back(input[i]);
			}
		};
};

#endif
