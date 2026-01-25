# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adnen <adnen@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/26 00:46:32 by adnen             #+#    #+#              #
#    Updated: 2026/01/26 00:46:34 by adnen            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- VARIABLES ---
NAME        = spider

CC          = g++
CFLAGS      = -Wall -Wextra -Werror -std=c++17

# Liste de tes sources (ajoute ici les futurs fichiers .cpp)
SRCS        = main.cpp spider.cpp

# Transformation automatique : .cpp devient .o
OBJS        = $(SRCS:.cpp=.o)

# --- COULEURS (Pour le style) ---
GREEN       = \033[1;32m
YELLOW      = \033[1;33m
RESET       = \033[0m

# --- RÈGLES ---

# 1. Règle par défaut (lance 'make' tout court)
all: $(NAME)

# 2. Création de l'exécutable (Liking)
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅ Compilation réussie ! Exécutable : ./$(NAME)$(RESET)"

# 3. Compilation des objets (.cpp -> .o)
%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

# 4. Supprime uniquement les fichiers .o
clean:
	@rm -f $(OBJS)
	@echo "$(YELLOW)🧹 Fichiers objets (.o) nettoyés.$(RESET)"

# 5. Supprime tout (.o + exécutable)
fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)✨ Nettoyage complet effectué.$(RESET)"

# 6. Recompile tout depuis zéro
re: fclean all

# 7. BONUS : Compile et supprime les .o direct (Usage : make pure)
pure: all clean

.PHONY: all clean fclean re pure