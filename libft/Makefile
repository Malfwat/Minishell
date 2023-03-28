NAME		=	../libft.a

H_FILES		=	libft.h

SRC			=	ft_atoi.c		\
				ft_bzero.c		\
				ft_calloc.c		\
				ft_isalnum.c	\
				ft_isalpha.c	\
				ft_isascii.c	\
				ft_isdigit.c	\
				ft_isprint.c	\
				ft_itoa.c		\
				ft_memchr.c		\
				get_next_line.c	\
				get_next_line_utils.c	\
				ft_memcmp.c		\
				ft_memcpy.c		\
				ft_memmove.c	\
				ft_memset.c		\
				ft_putchar_fd.c	\
				ft_putstr_fd.c	\
				ft_putendl_fd.c	\
				ft_putnbr_fd.c	\
				ft_strchr.c		\
				ft_strlcat.c	\
				ft_striteri.c	\
				ft_strjoin.c	\
				ft_strlcpy.c	\
				ft_strlen.c		\
				ft_split.c		\
				ft_strmapi.c	\
				ft_strdup.c		\
				ft_strncmp.c	\
				ft_strnstr.c	\
				ft_strchr.c		\
				ft_strtrim.c	\
				ft_strrchr.c	\
				ft_substr.c		\
				ft_tolower.c	\
				ft_toupper.c

SRC_DIR		=	srcs/

INCLUDES	=	includes/

SRC_BONUS	=	ft_lstnew.c			\
				ft_lstsize.c		\
				ft_lstlast.c		\
				ft_lstadd_front.c	\
				ft_lstadd_back.c	\
				ft_lstdelone.c		\
				ft_lstclear.c		\
				ft_lstiter.c		\
				ft_lstmap.c

OBJ	=	$(addprefix $(BUILD), $(SRC:.c=.o))

DEPS	=	$(OBJ:.o=.d)

BUILD		=	.build/

OBJ_BONUS	=	$(addprefix $(BUILD), $(SRC_BONUS:.c=.o))

LC			=	ar rc

CC			=	cc

CFLAGS		+= -Wall -Werror -Wextra

all:	$(NAME)

$(BUILD):
	@mkdir .build

$(BUILD)%.o:	$(SRC_DIR)%.c $(addprefix $(INCLUDES), $(H_FILES)) Makefile
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ -I./$(INCLUDES)

$(NAME):	$(BUILD) $(OBJ)
	@echo "————————————GENERATING—LIBFT.A————————————"
	@$(LC) $(NAME) $(OBJ)

bonus:	$(OBJ_BONUS) $(OBJ)
	@$(LC) $(NAME) $(OBJ) $(OBJ_BONUS)

clean:
	@rm -rf $(BUILD)

fclean:	clean
	@rm -f $(NAME)

re: fclean all

.PHONY: bonus clean fclean all re

-include $(DEPS)