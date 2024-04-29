// #ifndef	TESTSEMANTICACTIONS_HPP
// # define TESTSEMANTICACTIONS_HPP

// #include "SemanticAction.hpp"
// #include <stack>

// template<>
// class Extension<std::stack<std::string> >: public ASemanticAction<std::stack<std::string> > {
// 	public:
// 		void	execute(std::stack<std::string>& result);
// 		void	execute(std::stack<std::string>& result, Token const & token);
// };

// class Sum:Extension<std::stack<std::string> > {
// 	public:
// 		void	execute(std::stack<std::string>& result);
// 		void	execute(std::stack<std::string>& result, Token const & token);	
// }

// // template<>
// // class Product<std::stack<std::string> >: public ASemanticAction<std::stack<std::string> > {
// // 	public:
// // 		void	execute(std::stack<std::string>& result);
// // 		void	execute(std::stack<std::string>& result, Token const & token);
// // };

// // class Number: public ASemanticAction<std::stack<std::string> > {
// // 	public:
// // 		void	execute(std::stack<std::string>& result);
// // 		void	execute(std::stack<std::string>& result, Token const & token);
// // 		~Number();
// // };

// // class Factor: public ASemanticAction<std::stack<std::string> > {
// // 	public:
// // 		void	execute(std::stack<std::string>& result);
// // 		void	execute(std::stack<std::string>& result, Token const & token);
// // 		~Factor();
// // };

// #endif
