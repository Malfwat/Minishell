/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_ms.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:04:12 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/04 20:45:21 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_MS_H

# define STRUCT_MS_H

#include <stdbool.h>

typedef struct s_env_var
{
	char				*var;
	// char				*var_value;
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
	// struct s_cmd	*begin;
	// struct s_cmd	*end;
	// struct s_cmd	*next;
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

#endif