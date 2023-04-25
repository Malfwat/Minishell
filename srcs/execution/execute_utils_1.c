/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:13:08 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/21 21:08:25 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exec_ms.h>

void	close_sub_fds(t_block *head)
{
	while (head)
	{
		if (head->io_tab[1] != INIT_FD_VALUE)
			close(head->io_tab[1]);
		if (head->io_tab[0] != INIT_FD_VALUE)
			close(head->io_tab[0]);
		head = head->next;
	}
}

void	free_exec_vars(t_exec_vars exec_vars)
{
	free(exec_vars.argv);
	ft_strsfree(exec_vars.envp);
	ft_strsfree(exec_vars.path);
}

bool	rebuild_args(t_args **head, t_env_var *envp)
{
	t_args	*ptr;

	ptr = *head;
	while (ptr)
	{
		ptr->final_arg = join_splitted_arg(ptr->s_args, envp, true);
		if (errno)
			return (false);
		ptr = ptr->next;
	}
	return (true);
}

t_block	*find_next_block(t_block *block, bool ignore_sub)
{
	if (block->sub && !ignore_sub)
		return (block->sub);
	else if (block->pipe_next)
		return (block->pipe_next);
	else if (block->next)
		return (block->next);
	return (NULL);
}
