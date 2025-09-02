# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: qbeukelm <qbeukelm@student.42.fr>            +#+                      #
#                                                    +#+                       #
#    Created: 2025/08/11 09:30:12 by qbeukelm      #+#    #+#                  #
#    Updated: 2025/08/27 08:14:04 by quentinbeuk   ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# TODO: Explicitly name sources, avoid wildcards

BOLD				:= \033[1m
RED					:= \033[31;1m
GREEN				:= \033[32;1m
YELLOW				:= \033[33;1m
BLUE				:= \033[34;1m
RESET				:= \033[0m

# ---------------- Compiler ----------------
CXX       := c++
CXXFLAGS  := -std=c++17 -Wall -Wextra -Werror -MMD -MP -Iinc -Itests
# Uncomment for sanitizers while testing:
# SANFLAGS  := -fsanitize=address,undefined -fno-omit-frame-pointer
# CXXFLAGS += $(SANFLAGS)
# LDFLAGS  += $(SANFLAGS)

# ---------------- Targets -----------------
NAME      := webserv
TEST_BIN  := unit_tests

# ---------------- Dirs --------------------
DIR_OBJ   := obj
SRC_DIRS  := src
TEST_DIRS := tests

# ---------------- Sources -----------------
SOURCES    := $(sort $(shell find $(SRC_DIRS) -type f -name '*.cpp'))
OBJECTS    := $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(SOURCES))
DEPENDS    := $(OBJECTS:.o=.d)

# Any main*.cpp anywhere under src/ is excluded from the test link
MAIN_SRCS  := $(sort $(shell find $(SRC_DIRS) -type f -name 'main*.cpp'))
MAIN_OBJS  := $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(MAIN_SRCS))

LIB_OBJECTS  := $(filter-out $(MAIN_OBJS),$(OBJECTS))

# Test sources (any tests/**/test_*.cpp)
TEST_SOURCES := $(sort $(shell find $(TEST_DIRS) -type f -name 'test_*.cpp'))
TEST_OBJECTS := $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(TEST_SOURCES))
TEST_DEPENDS := $(TEST_OBJECTS:.o=.d)

# TEST_HELPERS := $(sort $(shell find $(TEST_DIRS) -type f -name '*.cpp' -not -name 'test_*.cpp'))
# TEST_HELPER_OBJECTS := $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(TEST_HELPERS))
# TEST_HELPER_DEPENDS := $(TEST_HELPER_OBJECTS:.o=.d)

# ---------------- Rules -------------------
all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "$(BLUE)\nLinking $(NAME)...$(RESET)"
	@$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "$(GREEN)\nDone\n$(RESET)"

# Build + run unit tests
test: $(TEST_BIN)
	@./$(TEST_BIN)

$(TEST_BIN): $(LIB_OBJECTS) $(TEST_OBJECTS)
	@echo "$(BLUE)\nLinking $(TEST_BIN)...$(RESET)"
	@$(CXX) $^ $(LDFLAGS) -o $@
	@echo "$(GREEN)\nUnit tests ready\n$(RESET)"

# Generic compile rule (app + tests)
$(DIR_OBJ)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(BLUE)\nCleaning ...$(RESET)"
	@rm -rf $(DIR_OBJ) $(TEST_BIN)
	@echo "$(GREEN)$(BOLD)\nAll clean!\n$(RESET)"

fclean: clean
	@rm -f $(NAME)

re: fclean all

-include $(DEPENDS) $(TEST_DEPENDS)

.PHONY: all clean fclean re test
