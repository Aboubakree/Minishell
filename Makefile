GREEN=\033[0;32m
NC=\033[0m # No Color

CC = @gcc
NAME = minishell

CFLAGS = -Wall -Werror -Wextra -g # -lreadline#-fsanitize=address

SRC =  minishell.c main.c environment.c \
	builtin/cd.c builtin/pwd.c builtin/env.c builtin/exit.c\
	# libft/ft_strlen.c libft/ft_substr.c libft/ft_strchr.c libft/ft_strncmp.c\
	# libft/ft_strdup.c libft/ft_split.c libft/ft_strjoin.c libft/ft_isdigit.c\
	# libft/ft_atoi.c\

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

.SECONDARY : $(OBJ)

