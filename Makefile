# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 18:07:52 by hateisse          #+#    #+#              #
#    Updated: 2023/04/25 20:32:47 by hateisse         ###   ########.fr        #
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

EXECUTION_DIR		=	execution/

EXECUTION			=	execute_cmd.c			\
						execute_utils_0.c		\
						execute_utils_1.c		\
						execute_built_ins.c		\
						children_functions.c	\
						init_exec_vars_io.c		\
						meta_char.c	

SIGNAL				=	manage_signals.c

SIGNAL_DIR			=	signals/

HISTORY				=	history_functions.c

HISTORY_DIR			=	history/

PROMPT				=	prompt_utils_0.c	\
						prompt_utils_1.c	\
						prompt_utils_2.c	\
						prompt_utils_3.c	\
						build_prompt_0.c	\
						build_prompt_1.c	\
						build_prompt_2.c

PROMPT_DIR			=	prompt/

BUILT_INS			=	cd.c	\
						echo.c	\
						exit.c	\
						pwd.c

BUILT_INS_DIR		=	built_ins/
		
ENV					=	export_unset_env.c	\
						init_env_var.c		\
						manage_env_var.c
				
ENV_DIR				=	env/

PARSING				=	input_output.c	\
						io_functions.c	\
						parenthesis.c	\
						utils_0.c		\
						utils_1.c		\
						utils_2.c		\
						check_cmd.c		\
						parsing.c		\
						wildcard_0.c	\
						wildcard_1.c	\
						word.c

PARSING_DIR			=	parsing/

STRUCT_UTILS		=	init_t_block.c			\
						init_t_args.c			\
						init_t_split_args.c		\
						free_structs_0.c		\
						free_structs_1.c		\
						manage_io_params.c

STRUCT_UTILS_DIR	=	struct_utils/

TERM_UTILS			=	term_params_handler.c	\
						exit_minishell.c		\
						init_shell.c

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
SRCS				+=	$(addprefix $(EXECUTION_DIR), $(EXECUTION))
SRCS				+=	$(addprefix $(TERM_UTILS_DIR), $(TERM_UTILS))
SRCS				+=	main.c

OBJ					=	$(addprefix $(BUILD), $(SRCS:.c=.o))

DEPS				= 	$(OBJ:.o=.d)

DIRS				=	$(addprefix $(BUILD), $(BUILT_INS_DIR))
DIRS				+=	$(addprefix $(BUILD), $(ENV_DIR))
DIRS				+=	$(addprefix $(BUILD), $(EXECUTION_DIR))
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

libft:
	@make -C libft

libft/libft.a: libft

suppr_script:
	@echo '{\nleak readline\nMemcheck:Leak\n...\nfun:readline\n}\n{\nleak add_history\nMemcheck:Leak\n...\nfun:add_history\n}' > suppr.txt

$(BUILD):
	mkdir $(BUILD) $(DIRS)
		
$(NAME):	libft/libft.a $(BUILD) $(OBJ)
	$(CC) -Wall -Werror -Wextra $(OBJ) $(LIB_DIR) -lft -lncurses -lreadline -o $(NAME)

$(BUILD)%.o:	$(SRCS_DIR)%.c Makefile 
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ 

launch:
	@make all
	@valgrind --leak-check=full --track-fds=yes --suppressions=suppr.txt ./minishell
# @valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=suppr.txt ./minishell

clean:
	@rm -rf $(BUILD)
	@echo "================cleaned================"

fclean:	clean
	@rm -f script.txt
	@make fclean -C libft
	@rm -rf $(NAME)
	@echo "================fcleaned==============="

re:	fclean all

.PHONY:	clean fclean all re libft

-include $(DEPS)