/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:15:17 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:20:38 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_exec.h>
#include <ms_env_function.h>
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

	if (!pid)
		return (true);
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

int	wait_children(void)
{
	int		status;
	t_pids	*children;

	children = g_ms_params.children;
	status = 0;
	if (children)
	{
		while (children)
		{
			if (waitpid(children->pid, &status, 0) == -1)
				return (-1);
			children = children->next;
		}
		free_children(&g_ms_params.children);
		g_ms_params.last_exit_code = status;
		set_env_exit_var(extract_exit_code(g_ms_params.last_exit_code));
	}
	return (g_ms_params.last_exit_code);
}
