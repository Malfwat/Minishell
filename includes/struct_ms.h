/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_ms.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:04:12 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/18 23:42:57 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_MS_H

# define STRUCT_MS_H

# include <stdbool.h>
# include <unistd.h>
# include <sys/types.h>
# include <termios.h>

typedef int	t_fd;
typedef struct s_env_var
{
	char				*var_name;
	char				*var_value;
	char				scope;
	bool				temp;
	struct s_env_var	*prev;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_split_arg
{
	char					scope;
	char					*str;
	struct s_split_arg		*next;
}	t_split_arg;

typedef struct s_args
{
	char			*final_arg;
	t_split_arg		*s_args;
	struct s_args	*prev;
	struct s_args	*next;
}	t_args;

typedef struct s_prompt_blocks
{
	int						type;
	int						delim_len;
	char					*str;
	struct s_prompt_blocks	*next;
}	t_prompt_blocks;

typedef struct s_cmd
{
	t_args			*args;
	char			*error_str;
	int				exit_value;
	pid_t			pid;
	char			*output;
	char			*input;
}	t_cmd;

typedef struct s_redirect
{
	bool				mode;
	char				*joined_name;
	t_split_arg			*file_name;
	t_split_arg			*hd_lim;
	t_fd				fd;
	int					errno_value;
	bool				append;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_block
{
	char			*subshell_command;
	char			*do_not_execute;
	int				operator;
	bool			input_source;
	t_redirect		*io_redirect;
	t_redirect		*heredoc;
	t_cmd			cmd;
	t_fd			io_tab[2];
	bool			io_is_overwritable[2];
	struct s_block	*pipe_next;
	struct s_block	*next;
	struct s_block	*sub;
}	t_block;

typedef struct s_prompt
{
	char		*git_branch_name;
	char const	*session_user;
	char		*cwd;
	int			width_without_mid_delim;
	int			last_exit_code;
	int			term_width;
	char		*time;
}	t_prompt;

typedef struct s_pids
{
	pid_t			pid;
	struct s_pids	*next;
}	t_pids;

typedef struct s_exec_vars
{
	char	**argv;
	char	**envp;
	char	**path;
}	t_exec_vars;

typedef struct s_minishell
{
	int				last_exit_code;
	char			*prev_line;
	t_prompt		prompt_params;
	t_env_var		*envp;
	t_pids			*children;
	t_block			*head;
	t_fd			history_fd;
	t_fd			stdin_fileno;
	struct termios	saved_params;
}	t_minishell;

#endif
