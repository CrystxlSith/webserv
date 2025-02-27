NAME = webserv

CC = c++ 

CFLAGS = -Wall -Wextra -Werror -std=c++98

MAIN = webserv

SRC = ${addsuffix .cpp, ${MAIN}} \
      server/Server.cpp

SRC_DIR = srcs

LIBFT = libft/libft.a

# Chemin complet vers les fichiers sources
SRCS = $(addprefix $(SRC_DIR)/, $(SRC))

OBJS = $(SRCS:.cpp=.o)

# Couleurs et styles
GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
RESET = \033[0m
BOLD = \033[1m

# Compteurs pour le pourcentage
TOTAL_FILES := $(words $(SRCS))
CURRENT_FILE = 0

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(GREEN)$(BOLD)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)$(BOLD)✓ $(NAME) compilé avec succès!$(RESET)"

$(LIBFT):
	@echo "$(BLUE)$(BOLD)Compilation de la libft...$(RESET)"
	@make --no-print-directory -C libft/
	@echo "$(GREEN)$(BOLD)✓ libft compilée avec succès!$(RESET)"

%.o: %.cpp
	@$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "$(BLUE)$(BOLD)Compilation [%d%%] %s$(RESET)\r" $$(( $(CURRENT_FILE) * 100 / $(TOTAL_FILES) )) $<
	@$(CC) $(CFLAGS) -c $< -o $@
	@if [ $(CURRENT_FILE) = $(TOTAL_FILES) ]; then echo ""; fi

clean:
	@echo "$(RED)$(BOLD)Nettoyage des fichiers objets...$(RESET)"
	@rm -f $(OBJS)
	@make --no-print-directory -C libft/ clean
	@echo "$(GREEN)$(BOLD)✓ Nettoyage terminé!$(RESET)"

fclean: clean
	@echo "$(RED)$(BOLD)Suppression de $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make --no-print-directory -C libft/ fclean
	@echo "$(GREEN)$(BOLD)✓ Suppression terminée!$(RESET)"

re: fclean all

.PHONY: all clean fclean re