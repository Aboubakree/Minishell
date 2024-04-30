NAME = minishell

CFLAGS = -Wall -Werror -Wextra -g #-fsanitize=address

SRC = main.c environment.c\
	libft/ft_strlen.c libft/ft_substr.c libft/ft_strchr.c libft/ft_strncmp.c\
	libft/ft_strdup.c libft/ft_split.c libft/ft_strjoin.c\
	builtin/cd.c builtin/pwd.c builtin/env.c\

OBJ = ${SRC:.c=.o}

all : $(NAME)

$(NAME) : $(OBJ)
	cc $(CFLAGS) -o $(NAME) $(OBJ) -lreadline

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.SECONDARY : $(OBJ)

