# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adnen <adnen@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/26 00:46:32 by adnen             #+#    #+#              #
#    Updated: 2026/02/08 14:27:30 by adnen            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- VARIABLES ---
NAME        = spider

# Liste tes fichiers ici
SRCS        = main.cpp spider.cpp Saver.cpp ErrorsHandler.cpp UrlHelper.cpp Downloader.cpp Parser.cpp

# Fichiers Headers
INCLUDES    = includes.hpp spider.hpp Saver.hpp

OBJS        = $(SRCS:.cpp=.o)

CC          = g++
FLAGS       = -Wall -Wextra -Werror -std=c++17

# [NOUVEAU] : On ajoute le flag pour linker la librairie curl
LIBS        = -lcurl

# --- COULEURS ---
GREEN       = \033[1;32m
RESET       = \033[0m

# --- RÈGLES ---

all: $(NAME)

# C'est ici que la magie opère
$(NAME): $(OBJS)
	@echo "$(GREEN)Création de l'exécutable $(NAME)...$(RESET)"
	# [MODIFIÉ] : On ajoute $(LIBS) à la fin de la commande
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LIBS)
	@echo "$(GREEN)Nettoyage automatique des fichiers objets (.o)...$(RESET)"
	@rm -f $(OBJS) 
	@echo "$(GREEN)✅ Terminé !$(RESET)"

# Compilation des .cpp en .o
%.o: %.cpp
	@$(CC) $(FLAGS) -c $< -o $@

# Clean ne sert plus à grand chose car on supprime déjà les .o, 
# mais on le garde par convention.
clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re