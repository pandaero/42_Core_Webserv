# Target executable
NAME	:= webserv

# Color definition for personalized status messages
COLOR	= \033[30m
RESET	= \033[0m

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
INC_PRE := Server.hpp Client.hpp Request.hpp Response.hpp
INC_DIR := include/
INC		:= $(addprefix $(INC_DIR), $(INC_PRE))

# Source files
SRC_PRE	:= main.cpp webserv.cpp Server.cpp Client.cpp Request.cpp Response.cpp ServerConfig.cpp
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
$(addprefix $(BUILD), main.o): $(addprefix $(SRC_DIR), main.cpp) $(INC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(COPT) $(DEFS) -c $< -o $@

# Make required object files
$(addprefix $(BUILD), %.o): $(addprefix $(SRC_DIR), %.cpp) $(addprefix $(INC_DIR), %.hpp) | $(BUILD_DIR)
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

# Add current folder and push with specific commit message
mgit:
	git add .
	@read -p "Enter the commit message: " msg; \
	git commit -m "$$msg"
	git push
	@echo "$(COLOR)git auto add & push with message performed.$(RESET)"

# State rules as non-files
.PHONY: all run directories clean fclean re
