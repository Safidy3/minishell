SRC = minishell.c ft_split_esc_2.c

# FLAGS = -Wall -Werror -Wextra -lreadline
# FLAGS = -Wall -Werror -Wextra
FLAGS =

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

run: all
	@./$(NAME)

debug: all
	@valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME)

.PHONY: all clean fclean re
