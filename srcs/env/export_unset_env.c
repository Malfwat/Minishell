/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:06:09 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/04 00:57:32 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <stdio.h>
#include <env_function.h>
#include <libft.h>
#include <stdlib.h>
#include <minishell.h>

bool	print_export(t_env *lst, t_fd fd)
{
	t_env	*cpy;
	char	**tab;
	int		i;

	cpy = sort_env(lst);
	if (!cpy)
		return (false);
	tab = build_envp(cpy);
	if (!tab)
		return (free_env_lst(cpy), false);
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
	ft_strsfree(tab);
	free_env_lst(cpy);
	return (true);
}

bool	export(char **tab, bool temp, t_fd fd)
{
	char	*name;
	int		i;
	bool	exit_value;

	i = -1;
	exit_value = true;
	if (!*tab)
		return (print_export(g_ms_params.envp, fd));
	while (tab && tab[++i])
	{
		name = get_env_name(tab[i]);
		if (!name)
			return (false);
		if (ft_strchr(name, '?'))
		{
			ms_perror("minishell: export", tab[i], "not a valid identifier");
			free(name);
			exit_value = false;
		}
		else if (!add_update_env_var(name, temp, tab[i]))
			return (false);
	}
	return (exit_value);
}

bool	unset(t_env **head, char **tab)
{
	t_env	*to_pop;
	int		i;

	i = -1;
	while (tab[++i])
	{
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
	return (true);
}

bool	env(t_env *lst, t_fd fd)
{
	char	**tab;
	int		i;

	tab = build_envp(lst);
	if (errno)
		return (false);
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
	return (true);
}
