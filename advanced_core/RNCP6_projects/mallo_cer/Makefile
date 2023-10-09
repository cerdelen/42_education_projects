CC = gcc
ALL_INC = $(addprefix -I, includes $(wildcard libs/*/includes))
CFLAGS = -Wall -Wextra -Werror $(ALL_INC)

SRC_DIR = src
OBJ_DIR = obj
# List of all source directories
SRC_DIRS := $(shell find $(SRC_DIR) -type d)

# Replace 'src' with 'obj' in source directories to create the corresponding object directories
OBJ_DIRS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC_DIRS))

SRC += $(wildcard $(SRC_DIR)/*.c)
SRC += $(wildcard $(SRC_DIR)/**/*.c)
		

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

TARGET = obj/libft_malloc_$(HOSTTYPE).so
LINK_TARGET = libft_malloc.so

all: $(OBJ_DIRS) $(TARGET)

$(OBJ_DIRS):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	@ar rsc $(TARGET) $(OBJ)
	@echo "\n\033[92m${TARGET} successfully created!\033[0m"
	@$(RM) $(LINK_TARGET)
	@ln -s $@ $(LINK_TARGET)
	@echo "\n\033[92mSym-link created!\033[0m"

clean:
	@rm -rf obj

fclean: clean
	@$(RM) ./a.out
	@$(RM) $(LINK_TARGET) $(TARGET)

re: fclean all

