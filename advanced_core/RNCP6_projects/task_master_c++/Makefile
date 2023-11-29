CC			=	c++
ALL_INC 	=	-Iincludes
CFLAGS		=	-Wall -Wextra -Werror $(ALL_INC)
SRC_DIR 	=	src
OBJ_DIR 	=	obj
SRC_DIRS	:=	$(shell find $(SRC_DIR) -type d)
OBJ_DIRS	:=	$(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC_DIRS))
SRC			+=	$(wildcard $(SRC_DIR)/*.cpp)
SRC			+=	$(wildcard $(SRC_DIR)/**/*.cpp)
OBJ			=	$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))


TARGET		=	TaskMaster

all: $(OBJ_DIRS) $(TARGET)

$(OBJ_DIRS):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)
	@echo "[92m${TARGET} successfully created![0m"

clean:
	@$(RM) -rf obj
	@echo "[92m${TARGET} successfully cleaned objects![0m"

fclean: clean
	@$(RM) $(TARGET)
	@echo "[92m${TARGET} successfully cleaned TARGET![0m"

re: fclean all
