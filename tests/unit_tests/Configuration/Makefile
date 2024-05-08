NAME            = configuration_test

SRC_DIR			= ../../../srcs
CONF_SRC_DIR	= ../../../srcs/configuration
MOCK_SRC_DIR	= ../../mock_srcs
INCLUDES_DIR	= ../../../includes
HPARSER_DIR		= ../../../includes/parsing/
HCONFIG_DIR		= ../../../includes/configuration/
HMOCK_DIR		= ../../mock_includes/
HRESPONSE_DIR	= ../../../includes/response/
SSRC_DIR		= ../../../srcs/parsing/

SOURCES         = configuration_test.cpp \
				  ../../mock_srcs/MockLogger.cpp \
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
				  ../../../srcs/response/RegexMatcher.cpp \
				  ../../../srcs/response/DefaultMatcher.cpp \
				  ../../../srcs/configuration/ConfigurationLoader.cpp \
				  ../../../srcs/configuration/ConfigurationBlock.cpp \
				  ../../../srcs/configuration/LocationBlock.cpp


OBJECTS         = $(SOURCES:.cpp=.o) $(MOCK_SOURCES:.cpp=.o)

CC              = c++
CPPFLAGS        = -Wall -Wextra -Werror -I$(SRC_DIR) -I$(INCLUDES_DIR) -I$(HPARSER_DIR) -I$(HRESPONSE_DIR) -I$(HCONFIG_DIR) -I$(HMOCK_DIR) -I$(SSRC_DIR)
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
	$(CC) $(CPPFLAGS) $(SANITIZE_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
