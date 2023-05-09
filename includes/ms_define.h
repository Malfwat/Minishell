/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_define.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 19:46:02 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/09 23:36:25 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_DEFINE_H
# define MS_DEFINE_H

# include <ms_struct.h>

extern t_minishell	g_ms_params;
typedef int			t_fd;

# define INIT_FD_VALUE -2

# define SUCCESS 0

// charset of single delimiter to check

# define SINGLE_DELIM " \t><|;"

// hex representing the -c flag in ms_params

# define C_FLAG 0x1

// indicate if we are going to open an input or output file

# define INPUT_MODE 0
# define OUTPUT_MODE 1

// indicate which line of the prompt is needed

# define P_HEADER 0
# define P_FOOTER 1

// input_source according to the last redirection encountered

# define HEREDOC 0
# define FILE_INPUT 1

// all defines related to the prompt (control sequence, decorative chars)

# define BOLD "\033[1m"
# define ITALIC "\033[3m"
# define RED "\033[38;5;9m"
# define LGREY_BG "\033[48;5;235m"
# define LGREY "\033[38;5;235m"
# define LLGREY "\033[38;5;243m"
# define GREEN "\033[38;5;10m"
# define LGREEN "\033[38;5;76m"
# define DGREEN "\033[38;5;22m"
# define LCYAN "\033[38;5;38m"
# define LBLUE "\033[38;5;110m"
# define ENDC "\033[0m"
# define R_ESC(sc) ("\001"sc"\002")
# define CORNER_LEFT_D "╰"
# define CORNER_LEFT_U "╭"

enum e_env_scope
{
	INTERNAL_VAR,
	PUBLIC_VAR,
	ANY_ENV_SCOPE
};

enum e_prompt_args_type
{
	P_GIT,
	P_TIME,
	P_CWD,
	P_USER,
	P_START_DELIM,
	P_MID_DELIM,
	P_EXIT_STATUS,
	P_END_DELIM
};

enum e_operator
{
	AND_OPERATOR = 1,
	OR_OPERATOR,
	PIPE_OPERATOR,
	SEMI_COLON
};

enum e_type
{
	ILLEGAL_INPUT = -8,
	ILLEGAL_OUTPUT,
	ILLEGAL_AOUTPUT,
	ILLEGAL_HEREDOC,
	EXPECTING_ARGUMENT,
	INCOMPLETE_INPUT_OUTPUT,
	INCOMPLETE_CMD_ARG,
	INCOMPLETE_PARENTHESIS,
	INPUT_OUTPUT,
	PARENTHESIS,
	CMD_ARG
};

enum e_errors
{
	CMD_SYNTAX_ERR
};

#endif /* MS_DEFINE_H */