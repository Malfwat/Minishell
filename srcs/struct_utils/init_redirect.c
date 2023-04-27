/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 03:42:35 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 03:46:00 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <struct_ms.h>
#include <stdlib.h>
#include <errno.h>
#include <ms_define.h>
#include <minishell.h>

t_redirect	*new_redirect(t_s_arg *arg, int mode)
{
	t_redirect	*new;

	new = ft_calloc(1, sizeof(t_redirect));
	if (!new)
		return (free_t_s_arg(&arg), NULL);
	if (!ft_strncmp(arg->str, "<<", 2))
		new->hd_lim = arg;
	else if (arg->str[0] == '<')
		new->file_name = arg;
	else if (!ft_strncmp(arg->str, ">>", 2))
	{
		new->append = true;
		new->file_name = arg;
	}
	else
		new->file_name = arg;
	new->mode = mode;
	new->fd = INIT_FD_VALUE;
	if (errno)
		return (NULL);
	return (new);
}

t_redirect	*last_redirect(t_redirect *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}
