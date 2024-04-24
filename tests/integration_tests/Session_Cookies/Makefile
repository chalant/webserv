NAME            = Session_Cookies_test

SOURCES         = Session_Cookies_test_main.cpp \
                  ../../../srcs/request/Request.cpp \
				  ../../../srcs/request/RequestParser.cpp \
				  ../../../srcs/connection/ConnectionManager.cpp \
				  ../../../srcs/response/Response.cpp \
				  ../../../srcs/constants/HttpHelper.cpp \
				  ../../../srcs/constants/HttpStatusCodeHelper.cpp \
				  ../../../srcs/constants/HttpMethodHelper.cpp \
				  ../../../srcs/constants/HttpHeaderHelper.cpp \
				  ../../../srcs/constants/HttpVersionHelper.cpp \
				  ../../../srcs/constants/LogLevelHelper.cpp

MOCK_SOURCES    = ../../mock_srcs/MockLogger.cpp \
				  ../../mock_srcs/MockFactory.cpp \
				  ../../mock_srcs/MockConfiguration.cpp \
				  ../../mock_srcs/MockConnection.cpp

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
