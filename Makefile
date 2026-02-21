# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adnen <adnen@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/21 19:55:50 by adnen             #+#    #+#              #
#    Updated: 2026/02/21 20:25:40 by adnen            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= spider

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++17
LIBS		= -lcurl

SRC_DIR		= ./
OBJ_DIR		= obj/

SRCS		= main.cpp Spider.cpp
OBJS		= $(SRCS:%.cpp=$(OBJ_DIR)%.o)

# Colors for pretty output
GREEN		= \033[0;32m
RED			= \033[0;31m
RESET		= \033[0m

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LIBS)
	@echo "$(GREEN)$(NAME) successfully built!$(RESET)"
	@$(MAKE) clean --no-print-directory

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled: $<"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Object files cleaned.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
