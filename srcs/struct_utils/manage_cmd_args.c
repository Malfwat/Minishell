/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:33:48 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/18 03:08:07 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <libft.h>
#include <stdlib.h>
#include <errno.h>

void	free_split_args(t_split_arg *lst);


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

void	insert_t_args(t_args **head, t_args *current, t_args *new_lst)
{
	if (*head == current)
		*head = new_lst;
	else
	{
		current->prev->next = new_lst;
		new_lst->prev = current->prev;
	}
	if (current->next)
		current->next->prev = last_args(new_lst);
	last_args(new_lst)->next = current->next;
	free(current->final_arg);
	free_split_args(current->s_args);
	free(current);
}

void	wc_update_t_args(t_args **args)
{
	t_args	*lst;
	t_args	*tmp;
	t_args	*wildcard;

	lst = *args;
	while (lst)
	{
		if (lst->final_arg && ft_strchr(lst->final_arg, '*'))
		{
			manage_wildcard(&wildcard, lst->final_arg);
			if (wildcard)
			{
				tmp = lst->next;
				insert_t_args(args, lst, wildcard);
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
