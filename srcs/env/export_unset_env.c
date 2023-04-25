/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:06:09 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/25 20:55:54 by hateisse         ###   ########.fr       */
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
	if (*lst == a)
		*lst = b;
	// a->next->prev = b;
	a->next = b->next;
	if (b->next)
		b->next->prev = a;
	b->next = a;
	b->prev = a->prev;
	if (a->prev)
		a->prev->next = b;
	a->prev = b;
}

void	print_export(t_env_var *lst, t_fd fd)
{
	t_env_var	*cpy;
	t_env_var	*tmp;

	cpy = cpy_t_env_var(lst);
	if (!cpy)
		return ;
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
	char	**tab;
	int		i;

	tab = build_envp(cpy);
	i = -1;
	if (fd == INIT_FD_VALUE)
		fd = 1;
	while (tab && tab[++i])
	{
		if (!ft_strncmp(tab[i], "?=", 2))
			continue ;
		ft_putstr_fd("export ", fd);
		ft_putendl_fd(tab[i], fd);
	}
	// free_env_lst(cpy);
}

void	export(t_minishell *ms_params, char **tab, bool temp, t_fd fd)
{
	t_env_var	*tmp;
	char		*name;
	char		*value;
	int			i;

	i = -1;
	if (!*tab)
		return (print_export(ms_params->envp, fd));
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
		tmp = find_env_var(ms_params->envp, name);
		if (!tmp)
		{
			if (!add_env_var(&ms_params->envp, name, value, temp))
				return (free_env_lst(ms_params->envp));
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
		if (ft_strchr(tab[i], '?'))
		{
			ms_perror("minishell: unset", tab[i], "not a valid identifier");
			continue ;
		}
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

void	env(t_env_var *lst, t_fd fd)
{
	char	**tab;
	int		i;

	tab = build_envp(lst);
	i = -1;
	if (fd == INIT_FD_VALUE)
		fd = 1;
	while (tab && tab[++i])
	{
		if (!ft_strncmp(tab[i], "?=", 2))
			continue ;
		ft_putendl_fd(tab[i], fd);
	}
	ft_strsfree(tab);
}
