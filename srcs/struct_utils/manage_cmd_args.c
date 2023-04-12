/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:33:48 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/12 14:59:22 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <libft.h>
#include <stdlib.h>
#include <errno.h>

bool		manage_wildcard(t_arg **head, char *str);



t_arg	*new_cmd_arg(char *arg)
{
	t_arg	*new;

	new = malloc(sizeof(t_arg));
	if (!new)
		return (NULL);
	new->name = arg;
	new->prev = NULL;
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
	{
		new->prev = last_arg(*head);
		new->prev->next = new;
	}
}

void	update_t_args(t_arg **args)
{
	t_arg	*lst;
	t_arg	*tmp;
	t_arg	*wildcard;

	lst = *args;
	while (lst)
	{
		if (ft_strchr(lst->name, '*'))
		{
			manage_wildcard(&wildcard, lst->name);
			if (wildcard)
			{
				if (!lst->prev)
				{
					*args = wildcard;
				}
				else
				{
					lst->prev->next = wildcard;
					wildcard->prev = lst->prev;
				}
				if (lst->next)
				{
					lst->next->prev = last_arg(wildcard);
				}
				last_arg(wildcard)->next = lst->next;
				tmp = lst->next;
				free(lst);
				lst = tmp;
			}
			else
				lst = lst->next;
			continue ;
		}
		else
			lst = lst->next;
	}
}

char	**build_argv(char *cmd, t_arg **head)
{
	char	**tab;
	int		len;
	t_arg	*tmp;

	len = 0;
	tmp = NULL;
	if (ft_strchr(cmd, '*'))
	{
		ft_addargs(&tmp, cmd);
		if (errno)
			return (NULL);
		update_t_args(&tmp);
		cmd = tmp->name;
		if (tmp->next)
		{
			last_arg(tmp)->next = *head;
			*head = tmp->next;
			tmp->next->prev = NULL;
			free(tmp);
		}
	}
	update_t_args(head);
	tmp = *head;
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
	tmp = *head;
	while (tmp)
	{
		tab[len++] = /* ft_strdup( */tmp->name/* ) */; // a voir si tu preferes utiliser strdup
		tmp = tmp->next;
	}
	tab[len] = NULL;
	return (tab);
}
