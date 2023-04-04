/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_ppx.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 21:17:08 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 16:34:00 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_PPX_H
# define STRUCT_PPX_H

typedef struct s_cmd
{
	char			**cmd;
	char			*error_str;
	int				exit_value;
	int				pid;
	struct s_cmd	*begin;
	struct s_cmd	*end;
	struct s_cmd	*next;
}t_cmds;

typedef struct s_task
{
	char	*files[2];
	char	*limiter;
	int		append;
	t_cmds	*cmd;
}t_task;

#endif /*STRUCT_PPX_H*/