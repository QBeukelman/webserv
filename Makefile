# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 09:30:12 by qbeukelm          #+#    #+#              #
#    Updated: 2025/08/11 10:22:48 by qbeukelm         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BOLD				:= \033[1m
RED					:= \033[31;1m
GREEN				:= \033[32;1m
YELLOW				:= \033[33;1m
BLUE				:= \033[34;1m
RESET				:= \033[0m

# Compiler
CXX 			:= c++
CXXFLAGS		:= -std=c++98 -Wall -Werror -Wextra -MMD -MP -Iinc

# Targets
NAME				:= webserv

# Directories
DIR_OBJ						:= obj
SRC_DIRS					:= src

# Sources
SOURCES  := $(sort $(shell find $(SRC_DIRS) -type f -name '*.cpp'))
OBJECTS  := $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(SOURCES))
DEPENDS  := $(OBJECTS:.o=.d)

all: $(NAME)

# Link
$(NAME): $(OBJECTS)
	@echo "$(BLUE)\nLinking $(NAME)...$(RESET)"
	@$(CXX) $(OBJECTS) -o $@
	@echo "$(GREEN)\nDone$(RESET)"

# Compile
$(DIR_OBJ)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(BLUE)\nCleaning ...$(RESET)"
	@rm -rf $(DIR_OBJ)
	@echo "$(GREEN)$(BOLD)\nAll clean!\n$(RESET)"

fclean: clean
	@rm -f $(NAME)

re: fclean all

-include $(DEPENDS)

.PHONY: all clean fclean re
