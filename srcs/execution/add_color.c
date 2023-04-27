/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 01:21:19 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 01:26:58 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <struct_ms.h>
#include <stdlib.h>
#include <libft.h>
#include <minishell.h>

bool	is_colorable(char *str)
{
	if (!ft_strcmp(str, "ls"))
		return (true);
	if (!ft_strcmp(str, "grep"))
		return (true);
	if (!ft_strcmp(str, "egrep"))
		return (true);
	if (!ft_strcmp(str, "fgrep"))
		return (true);
	if (!ft_strcmp(str, "diff"))
		return (true);
	if (!ft_strcmp(str, "ip"))
		return (true);
	return (false);
}

void	check_for_color(t_args **head)
{
	t_args	*tmp;

	if (is_colorable((*head)->final_arg))
	{
		if (!(*head)->next \
			|| ft_strncmp((*head)->next->final_arg, "--color=", 8))
		{
			tmp = new_cmd_arg(NULL);
			tmp->final_arg = ft_strdup("--color=auto");
			if (!tmp->final_arg)
				return ;
			tmp->prev = *head;
			if ((*head)->next)
				(*head)->next->prev = tmp;
			tmp->next = (*head)->next;
			(*head)->next = tmp;
		}
	}
}
