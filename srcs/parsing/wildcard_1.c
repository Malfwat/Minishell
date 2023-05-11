/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:31:37 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:13:59 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <dirent.h>
#include <sys/types.h>
#include <ms_struct.h>
#include <stdio.h>
#include <errno.h>
#include <libft.h>

bool	is_t_args_in_order(t_args *lst)
{
	char	*tmp;

	tmp = "";
	while (lst)
	{
		if (ft_strcasecmp(tmp, lst->final_arg) > 0)
			return (false);
		else if (ft_strcasecmp(tmp, lst->final_arg) == 0 \
		&& ft_strcmp(tmp, lst->final_arg) < 0)
			return (false);
		tmp = lst->final_arg;
		lst = lst->next;
	}
	return (true);
}

void	swap_args_node(t_args **lst, t_args *a, t_args *b)
{
	if (*lst == a)
		*lst = b;
	a->next = b->next;
	if (b->next)
		b->next->prev = a;
	b->next = a;
	b->prev = a->prev;
	if (a->prev)
		a->prev->next = b;
	a->prev = b;
}

t_args	*sort_args(t_args *lst)
{
	t_args	*tmp;

	while (!is_t_args_in_order(lst))
	{
		tmp = lst;
		while (tmp->next)
		{
			if (ft_strcasecmp(tmp->final_arg, tmp->next->final_arg) > 0)
				swap_args_node(&lst, tmp, tmp->next);
			else if (!ft_strcasecmp(tmp->final_arg, tmp->next->final_arg))
			{
				if (ft_strcmp(tmp->final_arg, tmp->next->final_arg) < 0)
					swap_args_node(&lst, tmp, tmp->next);
				else
					tmp = tmp->next;
			}
			else
				tmp = tmp->next;
		}
	}
	return (lst);
}

bool	compare_wildcard(char *pattern, char *str)
{
	int		i;
	char	**tab;
	char	*tmp;

	i = 0;
	tab = ft_split(pattern, '*');
	if (!tab || !*tab)
		return (ft_strsfree(tab), !errno);
	tmp = str;
	while (tab[i])
	{
		tmp = ft_strstr(tmp, tab[i]);
		if (!tmp)
			return (ft_strsfree(tab), false);
		if (i == 0)
			if (*pattern != '*' && *str != *tmp)
				return (ft_strsfree(tab), false);
		tmp += ft_strlen(tab[i++]);
	}
	if (tab[i])
		return (ft_strsfree(tab), false);
	if (pattern[ft_strlen(pattern) - 1] != '*' && *tmp)
		return (ft_strsfree(tab), false);
	return (ft_strsfree(tab), true);
}

bool	manage_wildcard(t_args **head, char *str)
{
	char	*path;
	char	*pattern;

	path = NULL;
	pattern = NULL;
	split_path_pattern(str, &path, &pattern);
	*head = wildcard(path, pattern);
	if (!(*head) && !errno)
		ft_add_t_args(head, ft_strdup(str));
	*head = sort_args(*head);
	free(pattern);
	free(path);
	return (true);
}
