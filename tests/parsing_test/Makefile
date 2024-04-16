NAME            = parser_test

SRC_DIR			= ../../srcs
INCLUDES_DIR	= ../../includes
HPARSER_DIR		= ../../includes/parsing/
SSRC_DIR		= ../../srcs/parsing/

SOURCES         = main.cpp \
				  TestSemanticActions.cpp \
				  ../../srcs/parsing/Grammar.cpp \
				  ../../srcs/parsing/GrammarRule.cpp \
				  ../../srcs/parsing/GrammarSymbol.cpp \
				  ../../srcs/parsing/SymbolMatching.cpp

OBJECTS         = $(SOURCES:.cpp=.o) $(MOCK_SOURCES:.cpp=.o)

CC              = c++
CPPFLAGS        = -Wall -Wextra -Werror -I$(SRC_DIR) -I$(INCLUDES_DIR) -I$(HPARSER_DIR) -I$(SSRC_DIR)
# Optional flags - uncomment to enable
CPPFLAGS += -std=c++98
#CPPFLAGS += -g
#CPPFLAGS += -fsanitize=address
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
