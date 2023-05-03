/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 03:07:36 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/03 05:16:26 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exec_ms.h>

void	close_sub_fds(t_block *head)
{
	while (head)
	{
		if (head->io_tab[1] > 2)
			close(head->io_tab[1]);
		if (head->io_tab[0] > 2)
			close(head->io_tab[0]);
		head = head->next;
	}
}

bool	rebuild_args(t_args **head, t_env *envp)
{
	t_args	*ptr;

	ptr = *head;
	while (ptr)
	{
		join_splitted_arg_test(&ptr, envp, ptr->s_args, true);
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

void	free_exec_vars(t_exec_vars exec_vars)
{
	free(exec_vars.cd_implicit);
	free(exec_vars.argv);
	ft_strsfree(exec_vars.envp);
	ft_strsfree(exec_vars.path);
}

t_args	*array_to_t_args(char **tab)
{
	int		size;
	t_args	*new;

	new = NULL;
	size = 0;
	while (tab[size])
	{
		ft_ls_t_args_addback(&new, NULL);
		if (errno)
			return (free_t_args(new), new);
		last_args(new)->final_arg = ft_strdup(tab[size++]);
	}
	return (new);
}

