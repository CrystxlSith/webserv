NAME = webserv

# Dossiers
OBJ_DIR = objs
SRC_DIR = srcs
INC_DIR = includes
DEP_DIR = deps

CC = c++ 

CFLAGS = -Wall -Wextra -Werror -std=c++98

MAIN = webserv

SRC = ${addsuffix .cpp, ${MAIN}} \
      server/Server.cpp \
      parsing/Config.cpp \
      utils/Utils.cpp \
      parsing/Locations.cpp \
      server/Request.cpp \
      server/Response.cpp \
      parsing/Files.cpp
            
SRC_DIR = srcs

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))

# Transformation des chemins pour mettre tous les objets dans un seul dossier
OBJ = $(SRC:.cpp=.o)
OBJ := $(subst /,_,$(OBJ))
OBJS = $(addprefix $(OBJ_DIR)/, $(OBJ))

# Fichiers de dépendances
DEPS = $(SRC:.cpp=.d)
DEPS := $(subst /,_,$(DEPS))
DEPS := $(addprefix $(DEP_DIR)/, $(DEPS))

# Couleurs et styles
GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
RESET = \033[0m
BOLD = \033[1m

# Compteurs pour le pourcentage
TOTAL_FILES := $(words $(SRCS))
CURRENT_FILE = 0

all: create_dirs $(NAME)

# Création des dossiers nécessaires (maintenant une cible plutôt qu'une directive shell)
create_dirs:
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)

$(NAME): $(OBJS)
	@echo "$(GREEN)$(BOLD)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(BOLD)✓ $(NAME) compilé avec succès!$(RESET)"

$(OBJ_DIR)/%_Server.o: $(SRC_DIR)/server/Server.cpp
	@$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "$(BLUE)$(BOLD)Compilation [%d%%] %s$(RESET)\r" $$(( $(CURRENT_FILE) * 100 / $(TOTAL_FILES) )) $<
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*_Server.d -c $< -o $@ -I$(INC_DIR)
	@if [ $(CURRENT_FILE) = $(TOTAL_FILES) ]; then echo ""; fi

$(OBJ_DIR)/%_Config.o: $(SRC_DIR)/parsing/Config.cpp
	@$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "$(BLUE)$(BOLD)Compilation [%d%%] %s$(RESET)\r" $$(( $(CURRENT_FILE) * 100 / $(TOTAL_FILES) )) $<
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*_Config.d -c $< -o $@ -I$(INC_DIR)
	@if [ $(CURRENT_FILE) = $(TOTAL_FILES) ]; then echo ""; fi

$(OBJ_DIR)/%_Utils.o: $(SRC_DIR)/utils/Utils.cpp
	@$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "$(BLUE)$(BOLD)Compilation [%d%%] %s$(RESET)\r" $$(( $(CURRENT_FILE) * 100 / $(TOTAL_FILES) )) $<
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*_Utils.d -c $< -o $@ -I$(INC_DIR)
	@if [ $(CURRENT_FILE) = $(TOTAL_FILES) ]; then echo ""; fi

$(OBJ_DIR)/%_Locations.o: $(SRC_DIR)/parsing/Locations.cpp
	@$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "$(BLUE)$(BOLD)Compilation [%d%%] %s$(RESET)\r" $$(( $(CURRENT_FILE) * 100 / $(TOTAL_FILES) )) $<
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*_Locations.d -c $< -o $@ -I$(INC_DIR)
	@if [ $(CURRENT_FILE) = $(TOTAL_FILES) ]; then echo ""; fi

$(OBJ_DIR)/webserv.o: $(SRC_DIR)/webserv.cpp
	@$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "$(BLUE)$(BOLD)Compilation [%d%%] %s$(RESET)\r" $$(( $(CURRENT_FILE) * 100 / $(TOTAL_FILES) )) $<
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/webserv.d -c $< -o $@ -I$(INC_DIR)
	@if [ $(CURRENT_FILE) = $(TOTAL_FILES) ]; then echo ""; fi

$(OBJ_DIR)/%_Request.o: $(SRC_DIR)/server/Request.cpp
	@$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "$(BLUE)$(BOLD)Compilation [%d%%] %s$(RESET)\r" $$(( $(CURRENT_FILE) * 100 / $(TOTAL_FILES) )) $<
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*_Request.d -c $< -o $@ -I$(INC_DIR)
	@if [ $(CURRENT_FILE) = $(TOTAL_FILES) ]; then echo ""; fi

$(OBJ_DIR)/%_Response.o: $(SRC_DIR)/server/Response.cpp
	@$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "$(BLUE)$(BOLD)Compilation [%d%%] %s$(RESET)\r" $$(( $(CURRENT_FILE) * 100 / $(TOTAL_FILES) )) $<
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*_Response.d -c $< -o $@ -I$(INC_DIR)
	@if [ $(CURRENT_FILE) = $(TOTAL_FILES) ]; then echo ""; fi

$(OBJ_DIR)/%_Files.o: $(SRC_DIR)/parsing/Files.cpp
	@$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "$(BLUE)$(BOLD)Compilation [%d%%] %s$(RESET)\r" $$(( $(CURRENT_FILE) * 100 / $(TOTAL_FILES) )) $<
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*_Files.d -c $< -o $@ -I$(INC_DIR)
	@if [ $(CURRENT_FILE) = $(TOTAL_FILES) ]; then echo ""; fi

clean:
	@echo "$(RED)$(BOLD)Nettoyage des fichiers objets...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@rm -rf $(DEP_DIR)
	@echo "$(GREEN)$(BOLD)✓ Nettoyage terminé!$(RESET)"

fclean: clean
	@echo "$(RED)$(BOLD)Suppression de $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)$(BOLD)✓ Suppression terminée!$(RESET)"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re