SRCS = minishell.c ft_split_esc_2.c \
		./builtins/ft_env.c ./builtins/ft_export.c ./builtins/ft_unset.c ./builtins/pwd_cd.c\
		./gnl/get_next_line.c ./gnl/get_next_line_utils.c \

# FLAGS = -Wall -Werror -Wextra -lreadline
FLAGS = -g -Wall -Werror -Wextra
RELINE_FLAG = -lreadline
# FLAGS = -g


OBJS = $(SRCS:.c=.o)
NAME = minishell

LIBFT_PATH = ./libft
LIBFT_ARCHIVE = $(LIBFT_PATH)/libft.a

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)
#@clear

$(NAME): $(LIBFT_ARCHIVE) $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT_ARCHIVE) -o $(NAME) $(RELINE_FLAG)

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

# stdin test from Makefile