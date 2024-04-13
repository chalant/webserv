#ifndef SEMANTICS_HPP
# define SEMANTICS_HPP

class ASemanticAction {
	public:
		ASemanticAction();
		virtual ~ASemanticAction() = 0;
		virtual	void	execute() = 0;
};

#endif
