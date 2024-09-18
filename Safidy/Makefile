SRC = $(GNL_PATH)/get_next_line_bonus.c $(GNL_PATH)/get_next_line_utils_bonus.c \
	echo.c

FLAGS = -Wall -Werror -Wextra

GNL_PATH = ./getNextLine

LIBFT_PATH = ./libft
LIBFT_ARCHIVE = $(LIBFT_PATH)/libft.a

OBJS = $(SRC:.c=.o)
NAME = minishell

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

.PHONY: all clean fclean re
