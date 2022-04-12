# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iltafah <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/10 14:58:16 by abdait-m          #+#    #+#              #
#    Updated: 2022/04/12 00:24:16 by iltafah          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

HDRS = ./headers/_server_.hpp ./headers/_UltimateHeader_.hpp \
		./headers/configParser.hpp ./headers/HttpRequest.hpp \
		./headers/IndexOfHtmlTemplate.hpp ./headers/HttpResponse.hpp \
		./headers/location.hpp ./headers/serverData.hpp \

SRCS =  ./sources/main.cpp ./sources/_server_.cpp \
		./sources/configParser.cpp ./sources/HttpRequest.cpp \
		./sources/IndexOfHtmlTemplate.cpp ./sources/HttpResponse.cpp \
		./sources/location.cpp ./sources/serverData.cpp \


OBJS = $(SRCS:.cpp=.o)


CPP = clang++

CPPFLAGS = -std=c++98 -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(OBJS) -o $@
	@echo "\n\033[35mExecutable file has been created .\033[0m\n"

%.o: %.cpp $(HDRS)
	$(CPP) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)
	@echo "\n\033[31mObjects removed!\033[0m\n"

fclean: clean
	@rm -f $(NAME)

re: fclean all