/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:06:09 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/24 14:40:21 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <stdio.h>
#include <env_function.h>
#include <libft.h>
#include <stdlib.h>
#include <minishell.h>

t_env_var	*cpy_t_env_var(t_env_var *lst)
{
	t_env_var	*new_lst;

	new_lst = NULL;
	while (lst)
	{
		if (!add_env_var(&new_lst, lst->var_name, lst->var_value, 0))
			return (free_env_lst(new_lst), NULL);
		lst = lst->next;
	}
	return (new_lst);
}

bool	is_in_order(t_env_var *lst)
{
	char	*tmp;

	tmp = "";
	while (lst)
	{
		if (ft_strcmp(tmp, lst->var_name) > 0)
			return (false);
		tmp = lst->var_name;
		lst = lst->next;
	}
	return (true);
}

void	swap_env_node(t_env_var **lst, t_env_var *a, t_env_var *b)
{
	t_env_var *a_prev;
	t_env_var *a_next;
	t_env_var *b_next;
	t_env_var *b_prev;

	a_prev = a->prev;
	a_next = a->next;
	b_next = b->next;
	b_prev = b->prev;
	if (a_prev)
		a_prev->next = b;
	if (a_next)
		a_next->prev = b;
	if (b_next)
		b_next->prev = a;
	if (b_prev)
		b_prev->next = a;
	if (*lst == a)
		*lst = b;
	else if (*lst == b)
		*lst = a;
	a->next = b_next;
	a->prev = b_prev;
	b->next = a_next;
	b->prev = a_prev;
}

void	print_export(t_env_var *lst)
{
	t_env_var	*cpy;
	t_env_var	*tmp;
	t_env_var	*tmp_next;

	cpy = cpy_t_env_var(lst);
	if (!cpy)
		return ;
	while (!is_in_order(cpy))
	{
		tmp = cpy;
		while (tmp && tmp->next)
		{
			tmp_next = tmp->next->next;
			if (ft_strcmp(tmp->var_name, tmp->next->var_name) > 0)
			{
				swap_env_node(&cpy, tmp, tmp->next);
				tmp = tmp_next;
			}
			else
				tmp = tmp->next;
		}
	}
	env(cpy);
	free_env_lst(cpy);
}

void	export(t_minishell *ms_params, t_env_var **lst, char **tab, bool temp)
{
	t_env_var	*tmp;
	char		*name;
	char		*value;
	int			i;

	i = -1;
	if (!*tab)
		return (print_export(*lst));
	while (tab && tab[++i])
	{
		name = get_env_var_name(tab[i]);
		if (!name)
			return ;
		if (ft_strchr(name, '?'))
		{
			ms_perror("minishell: export", tab[i], "not a valid identifier");
			ms_params->last_exit_code = 1;
			continue ;
		}
		value = get_env_var_value(tab[i]);
		tmp = find_env_var(*lst, name);
		if (!tmp)
		{
			if (!add_env_var(lst, name, value, temp))
				return (free_env_lst(*lst));
		}
		else
		{
			free(name);
			free(tmp->var_value);
			tmp->var_value = value;
		}
	}
}

void	unset(t_env_var **head, char **tab)
{
	t_env_var	*to_pop;
	int			i;

	i = -1;
	while (tab[++i])
	{
		if (!ft_strcmp(tab[i], "?"))
			continue ;
		to_pop = find_env_var(*head, tab[i]);
		if (!to_pop)
			continue ;
		if (to_pop->prev)
			to_pop->prev->next = to_pop->next;
		if (to_pop->next)
			to_pop->next->prev = to_pop->prev;
		if (to_pop == *head)
			*head = to_pop->next;
		free(to_pop->var_name);
		free(to_pop->var_value);
		free(to_pop);
	}
}

void	env(t_env_var *lst)
{
	char	**tab;
	int		i;

	tab = build_envp(lst);
	i = -1;
	while (tab && tab[++i])
	{
		if (!ft_strncmp(tab[i], "?=", 2))
			continue ;
		ft_putendl_fd(tab[i], 1);
	}
	ft_strsfree(tab);
}
