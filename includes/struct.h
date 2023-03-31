/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:04:12 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/31 20:50:16 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H

# define STRUCT_H

typedef struct s_cmd
{
	char			**cmd;
	bool			and_operator;
	char			*error_str;
	int				exit_value;
	int				pid;
	char			*output;
	char			*input;
	struct s_cmd	*begin;
	struct s_cmd	*end;
	// struct s_cmd	*next;
}	t_cmd;

typedef struct s_block
{
	struct s_block	*pipe_next;
	// char			*cmd_line;
	t_cmd			*cmd;
	struct s_block	*next;
	struct s_block	*sub;
}	t_block;

#endif