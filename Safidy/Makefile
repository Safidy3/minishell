
BUILTINS_SRCS = ./builtins/ft_env.c ./builtins/ft_export.c ./builtins/ft_unset.c ./builtins/pwd_cd.c\
		./builtins/ft_exit.c ./builtins/ft_echo.c

PARS_SRCS = ./parse_srcs/ft_split_esc_2.c ./parse_srcs/ft_split_esc.c ./parse_srcs/ft_env_parser.c

EXEC_SRCS = ./exec_srcs/ft_exec.c ./exec_srcs/ft_exec_builtins.c\
			./exec_srcs/ft_bin_path.c

REDIR_SRCS = ./redirection_srcs/ft_redirect.c

VALIDATOR_SRC = ./cmd_validation_src/ft_check_valid_commad.c

FREE_UTILS = ./free_utils/ft_free.c

SRCS =	ft_signals.c ft_heredoc.c ft_error.c\
		$(REDIR_SRCS) $(BUILTINS_SRCS) $(PARS_SRCS) $(EXEC_SRCS)\
		$(VALIDATOR_SRC) $(FREE_UTILS) main.c

FLAGS = -g -Wall -Werror -Wextra
RELINE_FLAG = -lreadline

OBJS = $(SRCS:.c=.o)
NAME = minishell

LIBFT_PATH = ./libft
LIBFT_ARCHIVE = $(LIBFT_PATH)/libft.a

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

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
	@valgrind --track-fds=yes --suppressions=readline.supp --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME)

.PHONY: all clean fclean re
