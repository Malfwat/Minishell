# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 18:07:52 by hateisse          #+#    #+#              #
#    Updated: 2023/05/08 01:35:19 by hateisse         ###   ########.fr        #
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

EXECUTION			=	execute_cmd.c				\
						execution.c					\
						flag_case.c					\
						add_color.c					\
						handle_parsed_user_input.c	\
						build_execve_args.c			\
						execute_utils_0.c			\
						execute_utils_1.c			\
						execute_utils_2.c			\
						execute_built_ins.c			\
						set_env_exit_var.c			\
						children_functions.c		\
						init_exec_vars_io.c			\
						meta_char.c	

SIGNAL				=	manage_signals.c

SIGNAL_DIR			=	signals/

HISTORY				=	history_functions.c

HISTORY_DIR			=	history/

SHELL_BANNER		=	display_shell_banner.c

SHELL_BANNER_DIR	=	shell_banner/

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
						export_utils.c		\
						init_env_var.c		\
						manage_env_var.c
				
ENV_DIR				=	env/

PARSING				=	ms_gnl.c			\
						input_output.c		\
						io_functions.c		\
						parenthesis.c		\
						utils_0.c			\
						utils_1.c			\
						utils_2.c			\
						utils_3.c			\
						check_cmd.c			\
						parsing.c			\
						heredoc.c			\
						heredoc_gnl.c		\
						parse_user_input.c	\
						wildcard_0.c		\
						wildcard_1.c		\
						word.c

PARSING_DIR			=	parsing/

STRUCT_UTILS		=	init_t_block.c			\
						init_t_args.c			\
						init_t_split_args.c		\
						init_redirect.c			\
						free_structs_0.c		\
						utils.c					\
						free_structs_1.c		\
						manage_io_params.c

STRUCT_UTILS_DIR	=	struct_utils/

TERM_UTILS			=	term_params_handler.c		\
						exit_minishell.c			\
						init_shell_0.c				\
						ensure_prompt_position.c	\
						print_usage.c

TERM_UTILS_DIR		=	term_utils/

MS_READLINE			=	ms_readline.c	\
						utils.c

MS_READLINE_DIR		=	ms_readline/

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
SRCS				+=	$(addprefix $(MS_READLINE_DIR), $(MS_READLINE))
SRCS				+=	$(addprefix $(SHELL_BANNER_DIR), $(SHELL_BANNER))
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
DIRS				+=	$(addprefix $(BUILD), $(MS_READLINE_DIR))
DIRS				+=	$(addprefix $(BUILD), $(SHELL_BANNER_DIR))

LGREY				=	\033[38;5;249m
LGREEN				=	\033[38;5;28m
LBLUE				=	\033[38;5;67m
NC					=	\033[0m
BLUE1				=	\033[38;5;72m
GO_END_LINE			=	\033[K
LPURPLE				=	\033[38;5;103m

SCRIPT_SUPPR		=	suppr.txt

################################################################################
#                                                                              #
#                                                                              #
#                                 FUNCTIONS                                    #
#                                                                              #
#                                                                              #
################################################################################

define makeprint
	echo -n "$(LGREY)$1>$(LPURPLE)$2$(NC)$(LGREY)[$(NC)$(LGREEN)$3$(LGREY)]$(NC)                                   \r"
endef

################################################################################
#                                                                              #
#                                                                              #
#                                   RULES                                      #
#                                                                              #
#                                                                              #
################################################################################

all:	$(NAME)

libft:
	@make -C libft > /dev/null

libft/libft.a: libft

suppr_script:
	@echo '{\nleak readline\nMemcheck:Leak\n...\nfun:readline\n}' > $(SCRIPT_SUPPR)
	@echo '{\nleak add_history\nMemcheck:Leak\n...\nfun:add_history\n}' >> $(SCRIPT_SUPPR)
	@echo '{\nleak tgetent_sp\nMemcheck:Leak\n...\nfun:tgetent_sp\n}' >> $(SCRIPT_SUPPR)
	@echo '{\nleak tgetstr_sp\nMemcheck:Leak\n...\nfun:tgetstr_sp\n}' >> $(SCRIPT_SUPPR)

$(BUILD):
	@mkdir $(BUILD) $(DIRS)

$(NAME):	libft/libft.a $(BUILD)  $(OBJ)
	@echo "$(LGREEN)OBJECT FILES COMPILED                                    $(NC)"
	@$(CC) -Wall -Werror -Wextra $(OBJ) $(LIB_DIR) -lft -lncurses -lreadline -o $(NAME)
	@echo "$(LGREEN)MINISHELL COMPILED                                        $(NC)"
	@tput cnorm

$(BUILD)%.o:	$(SRCS_DIR)%.c Makefile 
	@tput civis
	@$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ > /dev/null || $(call makeprint,"Compiling ", "$@" ,"KO")
	@$(call makeprint,"Compiling ", "$@" ,"OK")

launch: suppr_script
	@make all ; valgrind --leak-check=full --track-origins=yes --quiet --show-leak-kinds=all --track-fds=yes --suppressions=$(SCRIPT_SUPPR) ./minishell

clean:
	@rm -rf $(BUILD)
	@echo "================cleaned================"

fclean:	clean
	@rm -f suppr.txt
	@make fclean -C libft > /dev/null
	@rm -rf $(NAME)
	
re:	fclean all
	
.PHONY:	clean fclean all re libft

-include $(DEPS)