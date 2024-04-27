SRCS = srcs/ft_putchar.c \
	   srcs/ft_swap.c \
	   srcs/ft_putstr.c \
	   srcs/ft_strlen.c \
	   srcs/ft_strcmp.c

NAME = libft.a
OBJS= ${SRCS:.c=.o}
INCS = includes

.c.o:
	cc -Wall -Wextra -Werror -I ${INCS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	ar rcs ${NAME} ${OBJS}

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean:
	rm -f ${NAME}

re: fclean all
