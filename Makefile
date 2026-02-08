# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adnen <adnen@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/26 00:46:32 by adnen             #+#    #+#              #
#    Updated: 2026/02/08 16:03:10 by adnen            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- VARIABLES ---
NAME_SPIDER    = spider
NAME_SCORPION  = scorpion

# Sources Spider
SRCS_SPIDER    = Spider/main.cpp Spider/Spider.cpp Spider/Saver.cpp Spider/ErrorsHandler.cpp \
                 Spider/UrlHelper.cpp Spider/Downloader.cpp Spider/Parser.cpp

# Sources Scorpion
SRCS_SCORPION  = Scorpion/main.cpp Scorpion/Scorpion.cpp

# Fichiers Headers
INCLUDES       = Spider/includes.hpp Spider/Spider.hpp Spider/Saver.hpp \
                 Scorpion/Scorpion.hpp

OBJS_SPIDER    = $(SRCS_SPIDER:.cpp=.o)
OBJS_SCORPION  = $(SRCS_SCORPION:.cpp=.o)

CC             = g++
FLAGS          = -Wall -Wextra -Werror -std=c++17 -I./Spider -I./Scorpion

# Flag pour linker la librairie curl (uniquement pour spider)
LIBS_SPIDER    = -lcurl

# --- COULEURS ---
GREEN          = \033[1;32m
CYAN           = \033[1;36m
RESET          = \033[0m

# --- RÈGLES ---

all: $(NAME_SPIDER) $(NAME_SCORPION)

# Règle pour Spider
$(NAME_SPIDER): $(OBJS_SPIDER)
	@echo "$(GREEN)Création de l'exécutable $(NAME_SPIDER)...$(RESET)"
	@$(CC) $(FLAGS) $(OBJS_SPIDER) -o $(NAME_SPIDER) $(LIBS_SPIDER)
	@rm -f $(OBJS_SPIDER)
	@echo "$(GREEN)✅ Spider Terminé ! (Objets nettoyés)$(RESET)"

# Règle pour Scorpion
$(NAME_SCORPION): $(OBJS_SCORPION)
	@echo "$(CYAN)Création de l'exécutable $(NAME_SCORPION)...$(RESET)"
	@$(CC) $(FLAGS) $(OBJS_SCORPION) -o $(NAME_SCORPION)
	@rm -f $(OBJS_SCORPION)
	@echo "$(CYAN)✅ Scorpion Terminé ! (Objets nettoyés)$(RESET)"

# Compilation des .cpp en .o
%.o: %.cpp
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS_SPIDER) $(OBJS_SCORPION)
	@echo "Objets supprimés."

fclean: clean
	@rm -f $(NAME_SPIDER) $(NAME_SCORPION)
	@echo "Exécutables supprimés."

re: fclean all

.PHONY: all clean fclean re