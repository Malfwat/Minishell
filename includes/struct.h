/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 21:17:08 by malfwa            #+#    #+#             */
/*   Updated: 2023/01/27 18:53:07 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_cmd
{
	char			**cmd;
	char			*error_str;
	int				exit_value;
	int				pid;
	struct s_cmd	*begin;
	struct s_cmd	*end;
	struct s_cmd	*next;
}t_cmd;

typedef struct s_task
{
	char	*files[2];
	char	*limiter;
	int		append;
	t_cmd	*cmd;
}t_task;

#endif /*STRUCT_H*/