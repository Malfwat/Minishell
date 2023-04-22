/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:06:09 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 03:43:24 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <stdio.h>
#include <env_function.h>
#include <libft.h>
#include <minishell.h>

void	export(t_minishell *ms_params, t_env_var **lst, char **tab, bool temp)
{
	t_env_var	*tmp;
	char		*name;
	char		*value;
	int			i;

	i = -1;
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

	tab = build_envp(lst);
	ft_print_array_str(tab);
	ft_strsfree(tab);
}
