/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 18:19:03 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/07 20:13:49 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <stdlib.h>
#include <stdio.h>

void	free_t_args(t_arg *ptr)
{
	t_arg	*tmp;

	while (ptr)
	{
		tmp = ptr->next;
		free(ptr->name);
		free(ptr);
		ptr = tmp;
	}
}

void	free_cmd(t_cmd cmd)
{
	free(cmd.name);
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
		free(ptr->file_name);
		free(ptr->heredoc);
		free(ptr);
		ptr = tmp;
	}
}

void	flood_free(t_block *lst)
{
	if (!lst)
		return ;
	if (lst)
		printf("subshell: %s\n", lst->subshell_command);
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
