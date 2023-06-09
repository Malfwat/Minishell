/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 00:36:17 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:56:10 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_struct.h>
#include <stdbool.h>
#include <stdlib.h>
#include <libft.h>
#include <ms_env_function.h>
#include <minishell.h>

bool	is_in_order(t_env *lst)
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

void	swap_env_node(t_env **lst, t_env *a, t_env *b)
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

t_env	*sort_env(t_env *lst)
{
	t_env	*cpy;
	t_env	*tmp;

	cpy = cpy_t_env(lst);
	if (!cpy)
		return (NULL);
	while (!is_in_order(cpy))
	{
		tmp = cpy;
		while (tmp->next)
		{
			if (ft_strcmp(tmp->var_name, tmp->next->var_name) > 0)
				swap_env_node(&cpy, tmp, tmp->next);
			else
				tmp = tmp->next;
		}
	}
	return (cpy);
}

t_env	*cpy_t_env(t_env *lst)
{
	t_env	*new_lst;
	char	*name;
	char	*value;

	new_lst = NULL;
	while (lst)
	{
		name = ft_strdup(lst->var_name);
		value = ft_strdup(lst->var_value);
		if (errno)
			return (free(name), free(value), free_env_lst(new_lst), NULL);
		if (!add_env_var(&new_lst, name, value, lst->env_scope))
			return (free_env_lst(new_lst), NULL);
		lst = lst->next;
	}
	return (new_lst);
}

bool	add_update_env_var(char *name, bool env_scope, char *s)
{
	t_env	*tmp;
	char	*value;

	value = get_env_value(s);
	tmp = find_env_var(g_ms_params.envp, name);
	if (!tmp)
	{
		if (!add_env_var(&g_ms_params.envp, name, value, env_scope))
			return (free_env_lst(g_ms_params.envp), false);
	}
	else
	{
		free(name);
		free(tmp->var_value);
		if (env_scope == PUBLIC_VAR)
			tmp->env_scope = PUBLIC_VAR;
		tmp->var_value = value;
	}
	return (true);
}
