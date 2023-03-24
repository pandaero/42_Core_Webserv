# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pandalaf <pandalaf@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/22 10:20:16 by pandalaf          #+#    #+#              #
#    Updated: 2023/03/24 18:42:47 by pandalaf         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler options
CC		:= c++
CFLAGS	:= -Wall -Werror -Wextra -std=c++98

# Debugging options
ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif
ifeq ($(LEAKS), 1)
	CFLAGS += -fsanitize=address
endif

# Project directories
INC_DIR		:= ../include/
SRC_DIR		:= ../src/

# Test objects folder
BUILD		:= build/

# OS-Protection
ifeq ($(shell uname), Linux)
	BUILD_DIR	:= $(BUILD)
else
	BUILD_DIR	:= $(patsubst %/, %, $(BUILD))
endif

# URL tester
URL_TEST		:= url
URL_INC_PRE		:= Url.hpp
URL_SRC_PRE		:= Url.cpp
URL_INC			:= $(addprefix $(INC_DIR), $(URL_INC_PRE))
URL_SRC			:= $(addprefix $(SRC_DIR), $(URL_SRC_PRE))
URL_OBJ			:= $(addprefix $(BUILD), $(URL_SRC_PRE:.cpp=.o))
URL_TEST_SRC	:= Url_test.cpp
URL_TEST_OBJ	:= $(addprefix $(BUILD), $(URL_TEST_SRC:.cpp=.o))

# Compile all tests
all: url

# Run all tests
run: all
	./$(URL_TEST)

# Compile url test
url: $(URL_TEST)

# URL Test executable
$(URL_TEST): $(URL_OBJ) $(URL_TEST_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# URL Test objects from test src
$(addprefix $(BUILD), %.o): %.cpp | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# URL Test objects from project src
$(addprefix $(BUILD), %.o): $(addprefix $(SRC_DIR), %.cpp) $(addprefix $(INC_DIR), %.hpp) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Build directory
$(BUILD_DIR):
	mkdir -p $@

# Remove intermediate files
clean:
	rm -rf $(BUILD)

# Remove all non-source files
fclean: clean
	rm -f $(URL_TEST)

# Remove everything and make again
re: fclean all

# Rules not to be mistaken for files
.PHONY: all run url clean fclean re
