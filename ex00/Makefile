# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/03 08:48:37 by dhuss             #+#    #+#              #
#    Updated: 2025/05/07 12:39:55 by dhuss            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CPPFILES = main.cpp\
	BitcoinExchange.cpp
OFILES = $(CPPFILES:.cpp=.o)
FLAGS = -Wall -Werror -Wextra -std=c++17
NAME = btc

$(NAME) : $(OFILES)
	@$(CC) $(FLAGS) $(OFILES) -o $(NAME)
	@ echo "\033[32m$(NAME) created\033[0m"

%.o: %.cpp
	@$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

clean:
	@rm -rf $(OFILES)
	@ echo "\033[33mclean OFILES\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@ echo "\033[33mclean $(NAME)\033[0m"

re: fclean all

.PHONY: all re fclean clean