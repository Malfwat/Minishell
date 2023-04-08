/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 23:59:34 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/08 07:04:46 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <env_function.h>
#include <libft.h>
#include <stdlib.h>
#include <stdbool.h>

void	free_t_env(t_env_var *lst)
{
	t_env_var	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->var_name);
		free(lst->var_value);
		free(lst);
		lst = tmp;
	}
}

t_env_var	*get_last_env_var(t_env_var *tmp)
{
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

t_env_var	*new_env_var(char *name, char *value, bool temp)
{
	t_env_var	*new;

	if (!name)
		return (NULL);
	new = ft_calloc(1, sizeof(t_env_var));
	if (!new)
		return (NULL);
	new->var_name = name;
	new->var_value = value;
	new->temp = temp;
	return (new);
}

bool	add_env_var(t_env_var **head, char *name, char *value, bool temp)
{
	t_env_var	*new;
	t_env_var	*last;

	new = new_env_var(name, value, temp);
	if (!new)
		return (free(name), free(value), false);
	if (!*head)
		*head = new;
	else
	{
		last = get_last_env_var(*head);
		last->next = new;
		new->prev = last;
	}
	return (true);
}

t_env_var	*get_env_var(char **env)
{
	char		*name;
	char		*value;
	t_env_var	*lst;
	int			i;

	i = 0;
	lst = NULL;
	while (env && env[i])
	{
		name = get_env_var_name(env[i]);
		value = get_env_var_value(env[i]);
		if (!add_env_var(&lst, name, value, 0))
			return (free(name), free(value), free_t_env(lst), NULL);
		i++;
	}
	return (lst);
}
