SRCS	=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c\
			ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c\
			ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c\
			ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_split.c \
			ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c\
			ft_putnbr_base_fd.c ft_strrev.c ft_itoa_base.c ft_itoa_unsigned_base.c ft_join_char_str.c ft_join_str_char.c\
			ft_power.c ft_put_unsigned_nbr_base_fd.c ft_nbr_base_len.c ft_isspace.c ft_strcmp.c ft_strtrim.c

BONUS	=	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c\
			ft_lstadd_back.c ft_lstclear.c ft_lstiter.c ft_lstdelone.c

OBJS 	=	$(SRCS:.c=.o)
B_OBJS	=	$(BONUS:.c=.o)

NAME	=	libft2.a
B_NAME	=	libft.a
FLAGS	=	-Wall -Wextra -Werror
CC		=	gcc

.c.o:
	@${CC} ${FLAGS} -o ${<:.c=.o} -c $<

all: ${NAME}

${NAME}: ${OBJS}
	@ar rc ${NAME} ${OBJS}

clean:
	@rm -rf ${OBJS} $(B_OBJS)

fclean: clean
	@rm -f ${NAME} $(B_OBJS) ${NAME}

re: fclean all

bonus: $(B_NAME) 

$(B_NAME): $(OBJS) $(B_OBJS)
	@ar rcs $(NAME) $(OBJS) $(B_OBJS)

