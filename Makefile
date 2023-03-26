# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/17 19:39:35 by pandalaf          #+#    #+#              #
#    Updated: 2023/03/26 23:27:05 by wmardin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Target executable
NAME	:= webserv

# Default arguments
ifeq ($(shell uname), Linux)
	ARGS =
else
	ARGS =
endif

# Compiler Configuration
CC		:= c++
CFLAGS	:= -Wall -Werror -Wextra
COPT	:= -std=c++98

# Debug and leak-checking options. Usage: make DEBUG=1 LEAKS=1
ifeq ($(DEBUG), 1)
	COPT += -g
endif
ifeq ($(LEAKS), 1)
	COPT += -fsanitize=address
endif

# Include files
INC_PRE :=
INC_DIR := include/
INC		:= $(addprefix $(INC_DIR), $(INC_PRE))

# Source files
SRC_PRE	:= main.cpp
SRC_DIR	:= src/
SRC		:= $(addprefix $(SRC_DIR), $(SRC_PRE))

# Object files
BUILD	:= build/
OBJ_PRE	:= $(SRC_PRE:.cpp=.o)
OBJ		:= $(addprefix $(BUILD), $(OBJ_PRE))

# OS-Protection
ifeq ($(shell uname), Linux)
	BUILD_DIR	:= $(BUILD)
else
	BUILD_DIR	:= $(patsubst %/, %, $(BUILD))
endif

# RULES
all: $(NAME) directories

# Make and run the executable
run: all
	./$(NAME) $(ARGS)

# Make directories
directories: $(BUILD_DIR)

# Make target executable
$(NAME): $(OBJ) #$(addprefix $(BUILD), main.o)
	$(CC) $(CFLAGS) $(COPT) $^ -o $(NAME)

# Make the main object file
$(addprefix $(BUILD_DIR), main.o): $(addprefix $(SRC_DIR), main.cpp) $(INC) | $(BUILD)
	$(CC) $(CFLAGS) $(COPT) $(DEFS) -c $< -o $@

# Make required object files
$(addprefix $(BUILD_DIR), %.o): $(addprefix $(SRC_DIR), %.cpp) $(addprefix $(INC_DIR), %.hpp) $(addprefix $(INC_DIR), %.tpp) | $(BUILD)
	$(CC) $(CFLAGS) $(COPT) -c $< -o $@

# Make the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean intermediary files
clean:
	rm -rf $(BUILD_DIR)

# Clean all files
fclean: clean
	rm -rf $(NAME)

# Re-make everything
re: fclean all

# State rules as non-files
.PHONY: all run directories clean fclean re

# test run mulain branch
test:
	c++ src/main.cpp src/RequestHandler.cpp -Iinclude

# git add the entire folder w/o specific commit message
git:
	git add .
	git commit -m "auto add & push"
	git push
	@echo "$(COLOR)git auto add & push performed.$(RESET)"

# git add the entire folder with specific commit message
mgit:
	git add .
	@read -p "Enter the commit message: " msg; \
	git commit -m "$$msg"
	git push
	@echo "$(COLOR)git auto add & push with message performed.$(RESET)"
