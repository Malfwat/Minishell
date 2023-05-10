/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:06:09 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/10 18:42:19 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_struct.h>
#include <stdio.h>
#include <ms_env_function.h>
#include <libft.h>
#include <stdlib.h>
#include <minishell.h>

bool	print_export(t_env *lst, t_fd fd)
{
	t_env	*cpy;
	t_env	*tmp;

	cpy = sort_env(lst);
	if (!cpy)
		return (false);
	tmp = cpy;
	if (fd == INIT_FD_VALUE)
		fd = 1;
	while (cpy)
	{
		if (cpy->env_scope == PUBLIC_VAR)
		{
			ft_putstr_fd("export ", fd);
			ft_putstr_fd(cpy->var_name, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(cpy->var_value, fd);
		}
		cpy = cpy->next;
	}
	free_env_lst(tmp);
	return (true);
}

bool	change_scope(char *str)
{
	char	*name;
	t_env	*env_var;

	name = get_env_name(str);
	if (name)
	{
		env_var = find_env_var(g_ms_params.envp, name);
		free(name);
	}
	else
		env_var = find_env_var(g_ms_params.envp, str);
	if (env_var)
		env_var->env_scope = PUBLIC_VAR;
	return (true);
}

bool	is_valid_identifier(char *name)
{
	int	i;

	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (false);
	else
	{
		i = -1;
		while (name[++i])
		{
			if (!ft_isalnum(name[i]) && name[i] != '_')
				return (false);
		}
	}
	return (true);
}

bool	export(char **tab, bool env_scope, t_fd fd)
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
		if (errno)
			return (false);
		if (!name)
			change_scope(tab[i]);
		else if (!is_valid_identifier(name))
		{
			ms_perror("minishell: export", tab[i], "not a valid identifier");
			free(name);
			exit_value = false;
		}
		else if (!add_update_env_var(name, env_scope, tab[i]))
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
		if (to_pop == find_env_var(g_ms_params.envp, "OLDPWD"))
		{
			free(g_ms_params.previous_directory);
			g_ms_params.previous_directory = NULL;
		}
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
	if (fd == INIT_FD_VALUE)
		fd = 1;
	while (lst)
	{
		if (lst->env_scope == PUBLIC_VAR)
		{
			ft_putstr_fd(lst->var_name, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(lst->var_value, fd);
		}
		lst = lst->next;
	}
	return (true);
}
