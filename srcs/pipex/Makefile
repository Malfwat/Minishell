# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/28 14:02:40 by malfwa            #+#    #+#              #
#    Updated: 2023/04/03 15:53:03 by hateisse         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	pipex

NAME_B		=	pipex_b

################################################################################
#                                                                              #
#                                                                              #
#                                 VARIABLES                                    #
#                                                                              #
#                                                                              #
################################################################################

BUILD		=	.build/

SRC			=	free_func.c		\
				manage_file.c	\
				init.c			\
				pipex.c			\
				parsing.c		\
				lst.c			\
				child_0.c		\
				child_1.c

SRC_DIR		=	srcs/

INCLUDES	=	includes/

H_FILES		=	pipex.h

SRC_M		=	mandatory.c	

SRC_B		=	bonus.c		

OBJ			=	$(addprefix $(BUILD), $(SRC:.c=.o)) 

OBJ_M		=	$(addprefix $(BUILD), $(SRC_M:.c=.o))

OBJ_B		=	$(addprefix $(BUILD), $(SRC_B:.c=.o))

DEPS		=	$(OBJ:.o=.d)
DEPS		+=	$(OBJ_B:.o=.d)
DEPS		+=	$(OBJ_M:.o=.d)

LIBFT_A		=	libft.a

CC			=	cc

CFLAGS		+= -Wall -Wextra -Werror -g

################################################################################
#                                                                              #
#                                                                              #
#                                   RULES                                      #
#                                                                              #
#                                                                              #
################################################################################

all:	$(NAME)

$(BUILD):
	@mkdir $(BUILD)

$(BUILD)%.o:	$(SRC_DIR)%.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ -I./$(INCLUDES)

$(LIBFT_A):	libft_r

libft_r:
	@make -C libft

$(NAME):	$(LIBFT_A) $(BUILD) $(OBJ) $(OBJ_M)
	@echo "——————————————————PIPEX———————————————————" 
	$(CC) $(OBJ) $(OBJ_M) -o $@ -L./ -lft

bonus:	$(NAME_B)

$(NAME_B):	$(LIBFT_A) $(BUILD) $(OBJ) $(OBJ_B)
	@echo "——————————————————BONUS———————————————————" 
	@$(CC) $(OBJ) $(OBJ_B) -o $(NAME_B) -L./ -lft

clean:
	@rm -rf $(BUILD)
	@echo "—————————————CLEANING—LIBFT———————————————" 
	@make clean -C libft

fclean:	clean
	@echo "—————————————CLEANING—PIPEX———————————————" 
	@rm -f $(NAME)
	@rm -f $(NAME_B)
	@echo "—————————————ERASING—LIBFT.A——————————————" 
	@make fclean -C libft

re:	fclean all

.PHONY: all re clean fclean bonus libft_r

-include $(DEPS)