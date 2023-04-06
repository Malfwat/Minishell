/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_ms.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:04:12 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/06 21:49:33 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_MS_H

# define STRUCT_MS_H

# include <stdbool.h>

typedef struct s_env_var
{
	char				*var_name;
	char				*var_value;
	bool				temp;
	struct s_env_var	*prev;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_arg
{
	char			*name;
	struct s_arg	*next;
}	t_arg;

typedef struct s_cmd
{
	char			*name;
	t_arg			*args;
	char			*error_str;
	int				exit_value;
	int				pid;
	char			*output;
	char			*input;
}	t_cmd;

typedef struct s_redirect
{
	char				*file_name;
	char				*heredoc;
	bool				append;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_block
{
	char			*subshell_command;
	int				operator;
	t_redirect		*input_redirect;
	t_redirect		*output_redirect;
	t_cmd			cmd;
	struct s_block	*pipe_next;
	struct s_block	*prev;
	struct s_block	*next;
	struct s_block	*sub;
}	t_block;

typedef struct s_prompt
{
	char	*git_branch_name;
	char	*session_user;
	char	*cwd;
	int		width_without_mid_delim;
	int		last_exit_code;
	int		term_width;
	char	*time;
}	t_prompt;


#endif