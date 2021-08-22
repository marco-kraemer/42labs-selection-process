NAME = a.out

SRC_PATH = ./src

OBJ_PATH = ./obj

SRC = $(notdir $(wildcard ./src/*.c))
OBJ = $(addprefix $(OBJ_PATH)/, $(SRC:.c=.o))

CC = gcc

FLAGS = -Wall -Wextra -Werror

all: $(NAME) $(OBJ)

$(NAME): $(OBJ)
	@$(CC) libraries/mongoose.c libraries/mjson.c $(FLAGS) $(OBJ) -lmysqlclient -lcurl -ljson-c -o $(NAME)

$(OBJ_PATH)/%.o:	$(SRC_PATH)/%.c
	@$(CC) -g $(FLAGS) -c $< -o $@

fclean: clean
	@$(RM) $(NAME)

clean:
	@$(RM) $(OBJ_PATH)/*.o

re: fclean all

run: all
	@./$(NAME)

git:
	git add .
	git commit -m "Update"
	git push
