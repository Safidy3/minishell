NAME = minishell
FLAG = -Wall -Werror -Wextra

OBJ =$(SRC:.c=.o)

SRC = main.c ft_split_simple.c ft_split_separator.c ft_strjoin.c

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