SRCS = minishell.c ft_split_esc_2.c \
		./builtins/ft_env.c ./builtins/ft_export.c ./builtins/ft_unset.c

FLAGS = -Wall -Werror -Wextra
RDLINE_FLAG = -lreadline
# FLAGS = -g -Wall -Werror -Wextra
# FLAGS = -lreadline
# FLAGS = -g

OBJS = $(SRCS:.c=.o)
NAME = minishell

LIBFT_PATH = ./libft
LIBFT_ARCHIVE = $(LIBFT_PATH)/libft.a

%.o: %.c
	@cc $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(LIBFT_ARCHIVE) $(OBJS)
	@cc $(FLAGS) $(OBJS) $(LIBFT_ARCHIVE) -o $(NAME) $(RDLINE_FLAG)

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

norm:
	norminette $(SRCS)

debug: all
	@valgrind --suppressions=readline.supp --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME)
	@$(clean)

.PHONY: all clean fclean re
