/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 00:36:17 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 02:59:45 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <stdbool.h>
#include <stdlib.h>
#include <libft.h>
#include <env_function.h>
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

	new_lst = NULL;
	while (lst)
	{
		if (!add_env_var(&new_lst, lst->var_name, lst->var_value, 0))
			return (free_env_lst(new_lst), NULL);
		lst = lst->next;
	}
	return (new_lst);
}

bool	add_update_env_var(char *name, t_minishell *ms_prms, bool temp, char *s)
{
	t_env	*tmp;
	char	*value;

	value = get_env_value(s);
	tmp = find_env_var(ms_prms->envp, name);
	if (!tmp)
	{
		if (!add_env_var(&ms_prms->envp, name, value, temp))
			return (free_env_lst(ms_prms->envp), false);
	}
	else
	{
		free(name);
		free(tmp->var_value);
		tmp->var_value = value;
	}
	return (true);
}
