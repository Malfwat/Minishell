# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 18:07:52 by hateisse          #+#    #+#              #
#    Updated: 2023/04/08 08:08:02 by malfwa           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

################################################################################
#                                                                              #
#                                                                              #
#                                 VARIABLES                                    #
#                                                                              #
#                                                                              #
################################################################################

SRCS_DIR			=	srcs/

SIGNAL				=	manage_signals.c

SIGNAL_DIR			=	signals/

HISTORY				=	history_functions.c

HISTORY_DIR			=	history/

PROMPT				=	prompt_utils_0.c	\
						prompt_utils_1.c	\
						build_prompt_0.c	\
						build_prompt_1.c

PROMPT_DIR			=	prompt/

BUILT_INS			=	cd.c	\
						echo.c	\
						pwd.c

BUILT_INS_DIR		=	built_ins/
		
ENV					=	export_unset_env.c	\
						init_env_var.c		\
						manage_env_var.c
				
ENV_DIR				=	env/

PARSING				=	input_output.c	\
						io_functions.c	\
						parenthesis.c	\
						utils.c			\
						check_cmd.c		\
						parsing.c		\
						wildcard.c		\
						word.c

PARSING_DIR			=	parsing/

STRUCT_UTILS		=	init_t_block.c		\
						manage_cmd_args.c	\
						free_structs_0.c		\
						free_structs_1.c		\
						manage_io_params.c

STRUCT_UTILS_DIR	=	struct_utils/

TERM_UTILS			=	term_params_handler.c

TERM_UTILS_DIR		=	term_utils/

CC					=	cc

CFLAGS				+= -Wall -Werror -Wextra -MMD -MP -c -g3

INCLUDES			=	-I./includes/
INCLUDES			+=	-I./libft/
INCLUDES			+=	-I./libft/get_next_line/

LIB_DIR				=	-L./libft/ 

BUILD				= .build/

SRCS				=	$(addprefix $(STRUCT_UTILS_DIR), $(STRUCT_UTILS))
SRCS				+=	$(addprefix $(BUILT_INS_DIR), $(BUILT_INS))
SRCS				+=	$(addprefix $(PARSING_DIR), $(PARSING))
SRCS				+=	$(addprefix $(ENV_DIR), $(ENV))
SRCS				+=	$(addprefix $(PROMPT_DIR), $(PROMPT))
SRCS				+=	$(addprefix $(HISTORY_DIR), $(HISTORY))
SRCS				+=	$(addprefix $(SIGNAL_DIR), $(SIGNAL))
SRCS				+=	$(addprefix $(TERM_UTILS_DIR), $(TERM_UTILS))
SRCS				+=	test.c

OBJ					=	$(addprefix $(BUILD), $(SRCS:.c=.o))

DEPS				= 	$(OBJ:.o=.d)

DIRS				=	$(addprefix $(BUILD), $(BUILT_INS_DIR))
DIRS				+=	$(addprefix $(BUILD), $(ENV_DIR))
DIRS				+=	$(addprefix $(BUILD), $(HISTORY_DIR))
DIRS				+=	$(addprefix $(BUILD), $(SIGNAL_DIR))
DIRS				+=	$(addprefix $(BUILD), $(PROMPT_DIR))
DIRS				+=	$(addprefix $(BUILD), $(PARSING_DIR))
DIRS				+=	$(addprefix $(BUILD), $(STRUCT_UTILS_DIR))
DIRS				+=	$(addprefix $(BUILD), $(TERM_UTILS_DIR))

################################################################################
#                                                                              #
#                                                                              #
#                                   RULES                                      #
#                                                                              #
#                                                                              #
################################################################################

all:	$(NAME)

$(BUILD):
	mkdir $(BUILD) $(DIRS)
		
$(NAME):	$(BUILD) $(OBJ)
	$(CC) $(OBJ) $(LIB_DIR) -lft -lreadline -o $(NAME)

$(BUILD)%.o:	$(SRCS_DIR)%.c Makefile 
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ 

clean:
	rm -rf $(BUILD)

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	clean fclean all re

-include $(DEPS)