NAME        = spider

CC          = g++
CFLAGS      = -Wall -Wextra -Werror -std=c++17

# Liste de tes fichiers sources
SRCS        = main.cpp spider.cpp

# Transformation des .cpp en .o
OBJS        = $(SRCS:.cpp=.o)

# Règle par défaut
all: $(NAME)

# Compilation de l'exécutable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "✅ Compilation terminée : ./$(NAME)"

# Compilation des objets (.cpp -> .o)
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJS)
	@echo "🧹 Fichiers objets nettoyés"

fclean: clean
	rm -f $(NAME)
	@echo "✨ Exécutable supprimé"

re: fclean all

.PHONY: all clean fclean re