NAME            = CgiResponseGenerator_test

SOURCES         = CgiResponseGenerator_test_main.cpp \
                  ../../../srcs/response/CgiResponseGenerator.cpp \
				  ../../../srcs/constants/HttpStatusCodeHelper.cpp \
				  ../../../srcs/parsing/Grammar.cpp \
				  ../../../srcs/parsing/GrammarRule.cpp \
				  ../../../srcs/parsing/GrammarSymbol.cpp \
				  ../../../srcs/parsing/SymbolMatching.cpp \
				  ../../../srcs/parsing/Recognizer.cpp \
				  ../../../srcs/parsing/EarleyItem.cpp \
				  ../../../srcs/parsing/Tokenizer.cpp \
				  ../../../srcs/parsing/TerminalSymbol.cpp \
				  ../../../srcs/parsing/NonTerminalSymbol.cpp \
				  ../../../srcs/parsing/ParseTree.cpp \
				  ../../../srcs/parsing/Parser.cpp \
				  ../../../srcs/parsing/ParsingUtils.cpp \
				  ../../../srcs/parsing/Tokenizer.cpp \
				  ../../../srcs/configuration/ConfigurationLoader.cpp \
				  ../../mock_srcs/MockLogger.cpp \
				  ../../../srcs/configuration/ConfigurationBlock.cpp

MOCK_SOURCES    = ../../mock_srcs/MockRoute.cpp \
				  ../../mock_srcs/MockRequest.cpp \
				  ../../mock_srcs/MockLogger.cpp

INCLUDES        = -I../../../includes
MOCK_INCLUDES   = -I../../mock_includes

OBJECTS         = $(SOURCES:.cpp=.o) $(MOCK_SOURCES:.cpp=.o)

CC              = c++
CPPFLAGS        = -Wall -Wextra -Werror
# Optional flags - uncomment to enable
#CPPFLAGS += -std=c++98
CPPFLAGS += -g
#CPPFLAGS += -fsanitize=address
#CPPFLAGS += -O3

all: $(NAME)
	rm -f $(OBJECTS) 

$(NAME): $(OBJECTS)
	$(CC) $(CPPFLAGS) $(SANITIZE_FLAGS) $^ -o $@ $(INCLUDES) $(MOCK_INCLUDES)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(SANITIZE_FLAGS) -c $< -o $@ $(INCLUDES) $(MOCK_INCLUDES)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
