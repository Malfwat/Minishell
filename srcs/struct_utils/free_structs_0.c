/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:52:45 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/05 06:13:59 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_struct.h>
#include <stdlib.h>
#include <stdio.h>
#include <minishell.h>
#include <libft.h>

void	free_t_args(t_args *ptr)
{
	t_args	*tmp;

	while (ptr)
	{
		tmp = ptr->next;
		free(ptr->final_arg);
		free(ptr->cmd_w_path);
		free_t_s_arg(&ptr->s_args);
		free(ptr);
		ptr = tmp;
	}
}

void	free_cmd(t_cmd cmd)
{
	free(cmd.error_str);
	free(cmd.output);
	free(cmd.input);
	free_t_args(cmd.args);
}

void	free_redirect(t_redirect *ptr)
{
	t_redirect	*tmp;

	while (ptr)
	{
		tmp = ptr->next;
		free_t_s_arg(&ptr->file_name);
		free_t_s_arg(&ptr->hd_lim);
		free(ptr->joined_name);
		free(ptr);
		ptr = tmp;
	}
}

void	flood_free(t_block *lst)
{
	if (!lst)
		return ;
	free(lst->do_not_execute);
	free(lst->subshell_command);
	free_cmd(lst->cmd);
	free_redirect(lst->io_redirect);
	free_redirect(lst->heredoc);
	flood_free(lst->next);
	flood_free(lst->sub);
	flood_free(lst->pipe_next);
	lst->sub = NULL;
	free(lst);
}
