NAME            = Server_test

SOURCES         = Server_test_main.cpp \
				  ../../../srcs/request/RequestState.cpp \
                  ../../../srcs/network/Server.cpp \
				  ../../../srcs/constants/HttpStatusCodeHelper.cpp \
				  ../../../srcs/constants/HttpMethodHelper.cpp \
				  ../../../srcs/constants/HttpHeaderHelper.cpp \
				  ../../../srcs/constants/HttpVersionHelper.cpp \
				  ../../../srcs/constants/HttpHelper.cpp \
				  ../../../srcs/utils/Converter.cpp \
				  ../../../srcs/configuration/BlockList.cpp

MOCK_SOURCES    = ../../mock_srcs/MockLogger.cpp \
				  ../../mock_srcs/MockSocket.cpp \
				  ../../mock_srcs/MockSession.cpp \
				  ../../mock_srcs/MockPollfdManager.cpp \
				  ../../mock_srcs/MockConnectionManager.cpp \
				  ../../mock_srcs/MockConfigurationBlock.cpp \
				  ../../mock_srcs/MockRequest.cpp \
				  ../../mock_srcs/MockResponse.cpp \
				  ../../mock_srcs/MockConnection.cpp

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
