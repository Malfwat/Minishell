/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:33:48 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/15 20:26:03 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <libft.h>
#include <stdlib.h>
#include <errno.h>

bool		manage_wildcard(t_args **head, char *str);

t_args	*new_cmd_arg(t_split_arg *arg)
{
	t_args	*new;

	new = ft_calloc(1, sizeof(t_args));
	if (!new)
		return (NULL);
	new->s_args = arg;
	return (new);
}

t_args	*last_args(t_args *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

void	ft_ls_t_args_addback(t_args **head, t_split_arg *arg)
{
	t_args	*new;

	new = new_cmd_arg(arg);
	if (!new)
		return ;
	if (*head == NULL)
		*head = new;
	else
	{
		new->prev = last_args(*head);
		new->prev->next = new;
	}
}

void	ft_addarg_front(t_args **head, t_split_arg *arg)
{
	t_args	*new;

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

void	wc_update_t_args(t_args **args)
{
	t_args	*lst;
	t_args	*tmp;
	t_args	*wildcard;

	lst = *args;
	while (lst)
	{
		if (ft_strchr(lst->final_arg, '*'))
		{
			manage_wildcard(&wildcard, lst->final_arg);
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
					lst->next->prev = last_args(wildcard);
				}
				last_args(wildcard)->next = lst->next;
				tmp = lst->next;
				free(lst->final_arg);
				free(lst);
				lst = tmp;
			}
			else
				lst = lst->next;
		}
		else
			lst = lst->next;
	}
}

char	**build_argv(t_args **head)
{
	char	**tab;
	int		len;
	t_args	*tmp;

	len = 0;
	tmp = NULL;
	// ft_addarg_front(head, cmd);
	if (errno)
		return (NULL);
	wc_update_t_args(head);
	tmp = *head;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	tab = ft_calloc(len + 1, sizeof(char *));
	if (!tab)
		return (0);
	len = 0;
	tmp = *head;
	while (tmp)
	{
		tab[len++] = tmp->final_arg;
		tmp = tmp->next;
	}
	return (tab);
}
