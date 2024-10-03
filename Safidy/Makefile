SRC = minishel.c

FLAGS = -Wall -Werror -Wextra -lreadline

OBJS = $(SRC:.c=.o)
NAME = minishell

LIBFT_PATH = ./libft
LIBFT_ARCHIVE = $(LIBFT_PATH)/libft.a

%.o: %.c
	@$(CC) -g $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(LIBFT_ARCHIVE) $(OBJS)
	@$(CC) -g $(FLAGS) $(OBJS) $(LIBFT_ARCHIVE) -o $(NAME)
	@clear

$(LIBFT_ARCHIVE):
	@make bonus -C $(LIBFT_PATH)

clean:
	@rm -f $(OBJS)
	@make clean -C $(LIBFT_PATH)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_PATH)

re: fclean all

run: re
	@./minishell

debug: re
	@valgrind --leak-check=full --track-origins=yes ./minishell

.PHONY: all clean fclean re
