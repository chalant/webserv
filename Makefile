# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llapage <llapage@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 10:54:31 by llapage           #+#    #+#              #
#    Updated: 2024/04/10 14:18:25 by llapage          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
#-------------------SOURCES PATH----------------------
SOURCES     = src/
HEADER_PWD  = includes/
#-------------------COMPILATION----------------------
CC        	=   c++
FLAGS    	= 	-Wall -Werror -Wextra -g -std=c++98
#FLAGS   	=   -Wall -Werror -Wextra -g -fsanitize=address -std=c++98
#-------------------SOURCES FILES----------------------
HEADERS     =	ArequestHandler.hpp \
				CGIRequestHandler.hpp \
				IClientHandler.hpp \
				IConfiguration.hpp \
				IConfigurationLoader.hpp \
				DeleteMethodRequestHandler.hpp \
				ErrorPageRequestHandler.hpp \
				IExceptionHandler.hpp \
				GetMethodRequestHandler \
				ILogger.hpp \
				PollfdQueue.hpp \
				PostMethodRequestHandler.hpp \
				IRequest.hpp \
				RequestParser.hpp \
				Response.hpp \
				Router.hpp \
				Server.hpp \
				Sessions.hpp \
				StaticFileRequestHandler.hpp \
				UploadRequestHandler.hpp \
				WebservExceptions.hpp \

				
SRCS        =	main.cpp \
				ArequestHandler.cpp \
				CGIRequestHandler.cpp \
				IClientHandler.cpp \
				IConfiguration.cpp \
				DeleteMethodRequestHandler.cpp \
				ErrorPageRequestHandler.cpp \
				IExceptionHandler.cpp \
				GetMethodRequestHandler.cpp \
				ILogger.cpp \
				PollfdQueue.cpp \
				PostMethodRequestHandler.cpp \
				IRequest.cpp \
				RequestParser.cpp \
				Response.cpp \
				Router.cpp \
				Server.cpp \
				Sessions.cpp \
				StaticFileRequestHandler.cpp \
				UploadRequestHandler.cpp \
				
#-------------------OBJECTS----------------------
OBJS        =   $(SRCS:.cpp=.o)
#-------------------HEADERS----------------------
I_H_LIB     =   $(addprefix( -include, $(H_LIB)))
#-------------------COLORS-----------------------
RED         =   \x1b[31m
YELLOW      =   \x1b[33m
GREEN       =   \x1b[32m
NO_COLOR    =   \x1b[0m
BOLD        =   \x1b[1m
BOLD_OFF    =   \x1b[21m
#-------------------RULES-----------------------
%.o: %.cpp
			@printf "$(YELLOW)Generating $(NAME) objects... %-33.33s\r$(NO_COLOR)" $@
			@$(CC) $(FLAGS) -c $< -o $@
$(NAME):	$(OBJS)
			@printf "$(GREEN)Compiling $(NAME)... %33s\r$(NO_COLOR)" " "
			@$(CC) $(FLAGS) $(OBJS)  -o $(NAME)
			@echo "\n$(GREEN)$(BOLD)$@ done !$(BOLD_OFF)$(NO_COLOR)"
all:	$(NAME)

clean:
		@echo "$(RED)Deleting objects...$(NO_COLOR)"
		@rm -rf $(OBJS)
fclean:	clean
		@echo "$(RED)Deleting executables...$(NO_COLOR)"
		@rm -f $(NAME)
re:	fclean all
.PHONY: all clean fclean bonus re
