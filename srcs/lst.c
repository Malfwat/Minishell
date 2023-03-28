/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 21:43:44 by malfwa            #+#    #+#             */
/*   Updated: 2023/01/27 18:56:06 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

int	size_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		cmd = cmd->next;
		i++;
	}
	return (i);
}

void	f_err_get_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	if (cmd)
		cmd = cmd->begin;
	while (cmd)
	{
		tmp = cmd;
		if (cmd->cmd)
			free_tab(cmd->cmd);
		cmd = cmd->next;
		free(tmp);
	}
}

static t_cmd	*new_cmd(char **tab)
{
	t_cmd	*tmp;

	tmp = malloc(sizeof(*tmp));
	if (!tmp)
		return (NULL);
	tmp->cmd = tab;
	tmp->exit_value = 0;
	tmp->pid = -2;
	tmp->error_str = NULL;
	tmp->begin = NULL;
	tmp->next = NULL;
	tmp->end = tmp;
	return (tmp);
}

int	add_cmd(t_cmd **lst, char **tab)
{
	t_cmd	*tmp;

	tmp = new_cmd(tab);
	if (!tmp)
		return (f_err_get_cmd(*lst), free_tab(tab), 1);
	if (!*lst)
	{
		*lst = tmp;
		(*lst)->begin = tmp;
		return (0);
	}
	(*lst)->end->next = tmp;
	tmp->begin = (*lst)->begin;
	(*lst)->end = tmp;
	return (0);
}
