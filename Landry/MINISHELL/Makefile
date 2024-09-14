NAME = minishell
FLAG = -Wall -Werror -Wextra

OBJ =$(SRC:.c=.o)

SRC = main.c ft_split.c

all:$(NAME)

$(NAME):$(OBJ)
	cc -lreadline -o $(NAME) $(OBJ) -g
%.o:%.c
	cc -c  -g $< -o $@ 
clean:
	rm -f $(OBJ)
fclean:
	rm -f $(NAME)
re: fclean all
.PHONY: all clean fclean re