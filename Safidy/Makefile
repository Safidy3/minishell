
EXPORT_SRCS = ./builtins/export_srcs/ft_export.c\
			./builtins/export_srcs/ft_export_utils.c\
			./builtins/export_srcs/ft_export_error.c

ENV_SRCS = ./builtins/env_srcs/ft_env.c\
			./builtins/env_srcs/ft_env_utils.c\
			./builtins/env_srcs/ft_env_utils_2.c

BUILTINS_SRCS = $(EXPORT_SRCS) $(ENV_SRCS)\
				./builtins/ft_unset.c ./builtins/pwd_cd.c\
				./builtins/ft_exit.c ./builtins/ft_echo.c

ENV_PARSER = ./parse_srcs/env_parser/ft_env_parser.c\
			./parse_srcs/env_parser/ft_env_getter.c\
			./parse_srcs/env_parser/ft_env_setter.c

SPLIT_2_PARSER = ./parse_srcs/split_2_parser/ft_split_esc_2.c\
				./parse_srcs/split_2_parser/ft_split_2_words_ounter.c\
				./parse_srcs/split_2_parser/ft_split_2_utils.c\
				./parse_srcs/split_2_parser/ft_split_arg_type.c

VALIDATOR_SRC = ./cmd_validation_src/ft_check_valid_commad.c\
				./cmd_validation_src/ft_validator_utils.c\
				./cmd_validation_src/ft_validator_checker.c

HEREDOC_SRCS = ./heredoc_srcs/ft_heredoc.c\
				./heredoc_srcs/ft_heredoc_utils.c

PARS_SRCS = $(ENV_PARSER) $(SPLIT_2_PARSER)\
			./parse_srcs/ft_split_esc.c\
			./parse_srcs/ft_split_utils.c

EXEC_SRCS = ./exec_srcs/ft_exec.c ./exec_srcs/ft_exec_builtins.c\
			./exec_srcs/ft_bin_path.c ./exec_srcs/ft_exec_fd_utils.c\
			./exec_srcs/exec_error.c ./exec_srcs/exec_geter.c\
			./exec_srcs/exec_non_forked.c ./exec_srcs/exec_non_forked_heredoc.c

REDIR_SRCS = ./redirection_srcs/ft_redirect.c\
			./redirection_srcs/redirect_get_set.c

FREE_UTILS = ./utils/ft_free.c ./utils/ft_list_utils.c\
			./utils/ft_array_utils.c

SRCS =	ft_init.c ft_signals.c ft_error.c $(HEREDOC_SRCS)\
		$(REDIR_SRCS) $(BUILTINS_SRCS) $(EXEC_SRCS)\
		$(VALIDATOR_SRC) $(FREE_UTILS)\
		$(PARS_SRCS)\
		main.c

FLAGS = -Wall -Werror -Wextra
RELINE_FLAG = -lreadline
CC = gcc

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

debug: all
	@valgrind --track-fds=yes --suppressions=readline.supp --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME)
	@make clean

.PHONY: all clean fclean re
