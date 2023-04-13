/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:33:48 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/13 22:11:07 by hateisse         ###   ########.fr       */
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

	(void)arg;
	new = malloc(sizeof(t_args));
	if (!new)
		return (NULL);
	new->s_arg = arg;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

t_args	*last_args(t_args *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

void	ft_addargs(t_args **head, t_split_arg *arg)
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
					lst->next->prev = last_args(wildcard);
				}
				last_args(wildcard)->next = lst->next;
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

t_args	*check_word_param(char *str, int *i, int *type, t_split_arg	**arg)
{
	char		quotes;
	
	*i += pass_whitespaces(&str[*i]);
	while (str[*i] && !is_delim(&str[*i]))
	{
		quotes = 0;
		if (ft_strchr("'\"", str[*i]) && ft_strchr(&str[*i + 1], str[*i]))
		{
			quotes = str[*i];
			(*i)++;
		}
		(*i) += slice_next_part(&str[*i], arg, quotes);
		if (errno)
			return (false);
		// if (quotes)
		// 	(*i)++;
		if (str[*i] && !is_delim(&str[*i]))
			(*i)++;
	}
	if (*arg && !errno)
	{
		*type = CMD_ARG;
		return (true);
	}
	return (false);
}

char	**build_argv(t_split_arg *cmd, t_args **head)
{
	char	**tab;
	int		len;
	t_args	*tmp;

	len = 0;
	tmp = NULL;
	ft_addarg_front(head, cmd);
	if (errno)
		return (NULL);
	wc_update_t_args(head);
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
