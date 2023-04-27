/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:06:09 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 00:43:29 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <stdio.h>
#include <env_function.h>
#include <libft.h>
#include <stdlib.h>
#include <minishell.h>

void	print_export(t_env_var *lst, t_fd fd)
{
	t_env_var	*cpy;
	char		**tab;
	int			i;

	cpy = sort_env(lst);
	if (!cpy)
		return ;
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
}

void	export(t_minishell *ms_params, char **tab, bool temp, t_fd fd)
{
	char		*name;
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
		if (!add_update_env_var(name, ms_params, temp, tab[i]))
			return ;
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
