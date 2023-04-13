/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:33:48 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/13 14:25:44 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <libft.h>
#include <stdlib.h>
#include <errno.h>

bool		manage_wildcard(t_arg **head, char *str);



t_arg	*new_cmd_arg(t_split_arg *arg)
{
	t_arg	*new;

	(void)arg;
	new = malloc(sizeof(t_arg));
	if (!new)
		return (NULL);
	new->s_arg = arg;
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

void	ft_addargs(t_arg **head, t_split_arg *arg)
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

void	ft_addarg_front(t_arg **head, t_split_arg *arg)
{
	t_arg	*new;

	new = new_cmd_arg(arg);
	if (!new)
		return ;
	if (*head == NULL)
		*head = new;
	else
	{
		new->next = *head;
		(*head)->prev = new;
		*head = new;
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
		if (ft_strchr(0/*lst->name*/, '*'))
		{
			manage_wildcard(&wildcard, 0/*lst->name*/);
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
				free(0/*lst->name*/);
				free(lst);
				lst = tmp;
			}
			else
				lst = lst->next;
			// continue ;
		}
		else
			lst = lst->next;
	}
}

char	**build_argv(t_split_arg *cmd, t_arg **head)
{
	char	**tab;
	int		len;
	t_arg	*tmp;

	len = 0;
	tmp = NULL;
	ft_addarg_front(head, cmd);
	if (errno)
		return (NULL);
	update_t_args(head);
	tmp = *head;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	tab = malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (0);
	len = 0;
	tmp = *head;
	while (tmp)
	{
		tab[len++] = 0; //tmp->name; // a voir si tu preferes utiliser strdup
		tmp = tmp->next;
	}
	tab[len] = NULL;
	return (tab);
}
