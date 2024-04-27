NAME = minishell

CFLAGS = -Wall -Werror -Wextra -fsanitize=address

SRC = main.c\
	libft/ft_strlen.c libft/ft_substr.c libft/ft_strchr.c libft/ft_strncmp.c\

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

