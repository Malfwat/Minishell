/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:06:09 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 17:03:31 by amouflet         ###   ########.fr       */
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
		if (!add_env_var(&new_lst, lst->name, lst->value, 0))
			return (free_env_lst(new), NULL);
		lst = lst->next;
	}
	return (new_lst);
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
	a_prev->next = b;
	b_next->prev = a;
}

void	print_lst(t_env_var *lst)
{
	t_env_var	*cpy;

	cpy = cpy_t_env_var(lst);
	if (!cpy)
		return ;
}

void	export(t_minishell *ms_params, t_env_var **lst, char **tab, bool temp)
{
	t_env_var	*tmp;
	char		*name;
	char		*value;
	int			i;

	i = -1;
	if (!*tab)
		print_export(*lst);
	while (tab && tab[++i])
	{
		name = get_env_var_name(tab[i]);
		if (!name)
			return ;
		if (!ft_strcmp(name, "?"))
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
