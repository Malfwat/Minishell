/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:04:12 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/03 14:52:44 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H

# define STRUCT_H

#include <stdbool.h>

typedef struct s_env_var
{
	char				*var;
	char				*var;
	struct s_env_var	*prev;
	struct s_env_var	*next;
}	t_env_var;


typedef struct s_cmd
{
	char			**cmd;
	char			*error_str;
	int				exit_value;
	int				pid;
	char			*output;
	char			*input;
	struct s_cmd	*begin;
	struct s_cmd	*end;
	// struct s_cmd	*next;
}	t_cmd;

typedef struct s_redirect
{
	char				*file_name;
	bool				append;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_block
{
	char			*cmd_line;
	int				operator;
	t_redirect		*input_redirect;
	t_redirect		*output_redirect;
	t_cmd			*cmd;
	struct s_block	*pipe_next;
	struct s_block	*prev;
	struct s_block	*next;
	struct s_block	*sub;
}	t_block;

#endif