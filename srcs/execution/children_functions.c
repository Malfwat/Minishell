/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:15:17 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/21 15:59:29 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exec_ms.h>
#include <sys/wait.h>
#include <signal.h>

void	infanticides(t_pids *preys)
{
	while (preys)
	{
		kill(preys->pid, SIGTERM);
		preys = preys->next;
	}
}

bool	store_pid(pid_t pid, t_pids **nursery)
{
	t_pids	*new;
	t_pids	*lst;

	new = ft_calloc(1, sizeof(t_pids));
	if (!new)
		return (infanticides(*nursery), false);
	new->pid = pid;
	if (!*nursery)
	{
		*nursery = new;
		return (true);
	}
	else
	{
		lst = *nursery;
		while (lst->next)
			lst = lst->next;
		lst->next = new;
	}
	return (true);
}

void	free_children(t_pids **children)
{
	t_pids	*tmp;
	t_pids	*ptr;

	ptr = *children;
	while (ptr)
	{
		tmp = ptr->next;
		free(ptr);
		ptr = tmp;
	}
	*children = NULL;
}

int	wait_children(t_minishell *ms_params)
{
	int		status;
	t_pids	*children;

	children = ms_params->children;
	status = 0;
	if (children)
	{
		while (children)
		{
			if (waitpid(children->pid, &status, 0) == -1)
				return (-1);
			children = children->next;
		}
		free_children(&ms_params->children);
		ms_params->last_exit_code = status;
	}
	return (ms_params->last_exit_code);
}
