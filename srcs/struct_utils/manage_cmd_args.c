/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:33:48 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/08 07:39:53 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <stdlib.h>

t_arg	*new_cmd_arg(char *arg)
{
	t_arg	*new;

	new = malloc(sizeof(t_arg));
	if (!new)
		return (NULL);
	new->name = arg;
	new->next = NULL;
	return (new);
}

t_arg	*last_arg(t_arg *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

void	ft_addargs(t_arg **head, char *arg)
{
	t_arg	*new;

	new = new_cmd_arg(arg);
	if (!new)
		return ;
	if (*head == NULL)
		*head = new;
	else
		last_arg(*head)->next = new;
}

char	**build_argv(char *cmd, t_arg *head)
{
	char	**tab;
	int		len;
	t_arg	*tmp;

	len = 0;
	tmp = head;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	tab = malloc(sizeof(char *) * (len + 2));
	if (!tab)
		return (0);
	tab[0] = cmd;
	len = 1;
	while (head)
	{
		tab[len++] = head->name; // a voir si tu preferes utiliser strdup
		head = head->next;
	}
	tab[len] = NULL;
	return (tab);
}
