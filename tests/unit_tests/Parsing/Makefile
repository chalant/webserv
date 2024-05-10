NAME            = parser_test

SRC_DIR			= ../../../srcs
CONF_SRC_DIR	= ../../../srcs/configuration
MOCK_SRC_DIR	= ../../mock_srcs
INCLUDES_DIR	= ../../../includes
HPARSER_DIR		= ../../../includes/parsing/
HCONFIG_DIR		= ../../../includes/configuration/
HMOCK_DIR		= ../../mock_includes/
SSRC_DIR		= ../../../srcs/parsing/

SOURCES         = parsing_test_main.cpp \
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

OBJECTS         = $(SOURCES:.cpp=.o) $(MOCK_SOURCES:.cpp=.o)

CC              = c++
CPPFLAGS        = -I$(SRC_DIR) -I$(INCLUDES_DIR) -I$(HPARSER_DIR) -I$(HCONFIG_DIR) -I$(HMOCK_DIR) -I$(SSRC_DIR) -Wall -Wextra -Werror
# Optional flags - uncomment to enable
# CPPFLAGS += -std=c++98
CPPFLAGS += -g
# CPPFLAGS += -fsanitize=address
#CPPFLAGS += -O3

all: $(NAME)
	rm -f $(OBJECTS) 

$(NAME): $(OBJECTS)
	$(CC) $(CPPFLAGS) $(SANITIZE_FLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(SANITIZE_FLAGS) -I$(SRC_DIR) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
