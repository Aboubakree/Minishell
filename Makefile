NAME = minishell

CFLAGS = -Wall -Werror -Wextra

SRC = main.c

OBJ = ${SRC:.c=.o}

all : $(NAME)

$(NAME) : $(OBJ)
	cc $(CFLAGS) -o $(NAME) $(OBJ)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.SECONDARY : $(OBJ)

