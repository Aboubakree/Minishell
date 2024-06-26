GREEN=\033[0;32m
NC=\033[0m # No Color

CC = @gcc
NAME = minishell

CFLAGS = -Wall -Werror -Wextra -fPIC -g3 

SRC = main.c environment.c environment_2.c environment_3.c\
	builtin/cd.c builtin/pwd.c builtin/env.c builtin/exit.c builtin/unset.c\
	builtin/export.c builtin/export_add.c builtin/export_print.c builtin/echo.c\
	execution/execution.c execution/here_doc.c execution/one_cmd.c\
	execution/multiple_cmds.c execution/pipe_init.c execution/utils.c\
	execution/open_files.c execution/utils_1.c execution/utils_2.c\

LIBFT_DIR = lib/libft
LIBFT = $(LIBFT_DIR)/libft.a


OBJ = ${SRC:.c=.o}

all : $(NAME)

# $(NAME) : $(OBJ) $(LIBFT)
# 	cc $(CFLAGS) -o $(NAME) $(OBJ)

$(NAME) : $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline
	@echo "\033[0;32m✅ minishell compiled\033[0m"
$(LIBFT):
	@make -C $(LIBFT_DIR)
	@make bonus -C $(LIBFT_DIR)
	@echo "\033[0;32m✅ libft compiled\033[0m"

clean :
	@make clean -C $(LIBFT_DIR)
	@rm -f $(OBJ)
	@echo "\033[0;32m🧹 cleaned\033[0m"

fclean : clean
	@make fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)
	@echo "\033[0;32m🧹🧹 fcleaned\033[0m"

re : fclean all
f : $(NAME)
	valgrind --leak-check=full ./$(NAME)

v : $(NAME)
	valgrind ./$(NAME)
k : $(NAME)
	./$(NAME)

.SECONDARY : $(OBJ)

