SRCS = minishell.c ft_split_esc_2.c ft_split_esc.c\
		./builtins/ft_env.c ./builtins/ft_export.c ./builtins/ft_unset.c ./builtins/pwd_cd.c\
		ft_signals.c main.c ft_free.c ft_exec.c ft_redirect.c ft_exec_builtins.c ft_heredoc.c ft_error.c\
		./builtins/ft_exit.c ./builtins/ft_echo.c ft_bin_path.c ft_check_valid_commad.c

# FLAGS = -g
FLAGS = -g -Wall -Werror -Wextra
RELINE_FLAG = -lreadline


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

debug: fclean all
	@valgrind --suppressions=readline.supp --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME)

.PHONY: all clean fclean re

# stdin test from Makefile