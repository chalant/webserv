NAME            = Buffer_test

SOURCES         = Buffer_test_main.cpp \
                  ../../../srcs/buffer/BufferManager.cpp \
			      ../../../srcs/buffer/FileBuffer.cpp \
				  ../../../srcs/buffer/SocketBuffer.cpp \
				  ../../../srcs/logger/Logger.cpp \
				  ../../../srcs/core/EventManager.cpp \
				  ../../../srcs/constants/LogLevelHelper.cpp

MOCK_SOURCES    = ../../mock_srcs/MockPollfdManager.cpp \
				  ../../mock_srcs/MockSocket.cpp \
				  ../../mock_srcs/MockServer.cpp \
				  ../../mock_srcs/MockRequestHandler.cpp \
				  ../../mock_srcs/MockLoggerConfiguration.cpp \
				  ../../mock_srcs/MockConfiguration.cpp

INCLUDES        = -I../../../includes
MOCK_INCLUDES   = -I../../mock_includes

OBJECTS         = $(SOURCES:.cpp=.o) $(MOCK_SOURCES:.cpp=.o)

CC              = c++
CPPFLAGS        = -Wall -Wextra -Werror
# Optional flags - uncomment to enable
#CPPFLAGS += -std=c++98
CPPFLAGS += -g
CPPFLAGS += -fsanitize=address
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
