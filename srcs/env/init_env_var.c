/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 23:59:34 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 04:11:48 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_env_function.h>
#include <libft.h>
#include <minishell.h>
#include <stdlib.h>
#include <stdbool.h>

t_env	*get_last_env(t_env *tmp)
{
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

t_env	*new_env_var(char *name, char *value, bool env_scope)
{
	t_env	*new;

	if (!name)
		return (NULL);
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		return (NULL);
	new->var_name = name;
	new->var_value = value;
	new->env_scope = env_scope;
	return (new);
}

bool	add_env_var(t_env **head, char *name, char *value, bool env_scope)
{
	t_env	*new;
	t_env	*last;

	new = new_env_var(name, value, env_scope);
	if (!new)
		return (free(name), free(value), false);
	if (!*head)
		*head = new;
	else
	{
		last = get_last_env(*head);
		last->next = new;
		new->prev = last;
	}
	return (true);
}

t_env	*get_env(char **env)
{
	char	*name;
	char	*value;
	t_env	*lst;
	int		i;

	i = 0;
	lst = NULL;
	while (env && env[i])
	{
		name = get_env_name(env[i]);
		value = get_env_value(env[i]);
		if (!name || !value || !add_env_var(&lst, name, value, PUBLIC_VAR))
			return (free(name), free(value), free_env_lst(lst), NULL);
		i++;
	}
	name = ft_strdup("?");
	value = ft_strdup("0");
	if (!name || !value || !add_env_var(&lst, name, value, INTERNAL_VAR))
		return (free(name), free(value), free_env_lst(lst), NULL);
	return (lst);
}
