NAME            = UploadResponseGenerator_test

SRC_DIR			= ../../../srcs
CONF_SRC_DIR	= ../../../srcs/configuration
MOCK_SRC_DIR	= ../../mock_srcs
INCLUDES_DIR	= ../../../includes
HPARSER_DIR		= ../../../includes/parsing/
HCONFIG_DIR		= ../../../includes/configuration/
HMOCK_DIR		= ../../mock_includes/
HRESPONSE_DIR	= ../../../includes/response/
SSRC_DIR		= ../../../srcs/parsing/

SOURCES         = UploadResponseGenerator_test_main.cpp \
				../../../mock_srcs/MockLogger.cpp \
				../../../srcs/buffer/BufferManager.cpp \
				../../../srcs/buffer/FileBuffer.cpp \
				../../../srcs/buffer/SocketBuffer.cpp \
				../../../srcs/utils/Converter.cpp \
				../../../srcs/utils/SignalHandler.cpp \
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
				../../../srcs/configuration/ConfigurationBlock.cpp \
				../../../srcs/configuration/Defaults.cpp \
				../../../srcs/configuration/ConfigurationLoader.cpp \
				../../../srcs/configuration/BlockList.cpp \
				../../../srcs/constants/HttpHeaderHelper.cpp \
				../../../srcs/constants/HttpHelper.cpp \
				../../../srcs/constants/HttpMethodHelper.cpp \
				../../../srcs/constants/HttpStatusCodeHelper.cpp \
				../../../srcs/constants/HttpVersionHelper.cpp \
				../../../srcs/constants/LogLevelHelper.cpp \
				../../../srcs/core/EventManager.cpp \
				../../../srcs/core/PollingService.cpp \
				../../../srcs/exception/ExceptionHandler.cpp \
				../../../srcs/factory/Factory.cpp \
				../../../srcs/logger/Logger.cpp \
				../../../srcs/logger/LoggerConfiguration.cpp \
				../../../srcs/network/Server.cpp \
				../../../srcs/network/Socket.cpp \
				../../../srcs/connection/RequestHandler.cpp \
				../../../srcs/connection/ClientHandler.cpp \
				../../../srcs/connection/Connection.cpp \
				../../../srcs/connection/ConnectionManager.cpp \
				../../../srcs/connection/Session.cpp \
				../../../srcs/pollfd/PollfdManager.cpp \
				../../../srcs/pollfd/PollfdQueue.cpp \
				../../../srcs/request/Request.cpp \
				../../../srcs/request/RequestParser.cpp \
				../../../srcs/response/CgiResponseGenerator.cpp \
				../../../srcs/response/UploadResponseGenerator.cpp \
				../../../srcs/response/RegexMatcher.cpp \
				../../../srcs/response/Response.cpp \
				../../../srcs/response/Router.cpp \
				../../../srcs/response/TempRouter.cpp \
				../../../srcs/response/Route.cpp \
				../../../srcs/response/StaticFileResponseGenerator.cpp \

OBJECTS         = $(SOURCES:.cpp=.o) $(MOCK_SOURCES:.cpp=.o)

CC              = c++
CPPFLAGS        = -I$(SRC_DIR) -I$(INCLUDES_DIR) -I$(HPARSER_DIR) -I$(HRESPONSE_DIR) -I$(HCONFIG_DIR) -I$(HMOCK_DIR) -I$(SSRC_DIR)
# Optional flags - uncomment to enable
# CPPFLAGS += -Wall -Wextra -Werror -std=c++98
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
