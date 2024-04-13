#if !defined(TESTSEMANTICACTIONS_HPP)
#define TESTSEMANTICACTIONS_HPP

#include "SemanticAction.hpp"

class Sum: public ASemanticAction {
	public:
		Sum(ASemanticAction& left, ASemanticAction& right);
};

class Product: public ASemanticAction {
	public:
		Product(ASemanticAction& action1, ASemanticAction& action2);
};

class Number: public ASemanticAction {
	
};

class UnaryAction: public ASemanticAction 
{
	public:
		UnaryAction(ASemanticAction& action);
};

#endif
