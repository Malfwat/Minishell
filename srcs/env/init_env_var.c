/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 23:59:34 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 01:28:27 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <env_function.h>
#include <libft.h>
#include <stdlib.h>
#include <stdbool.h>

void	free_env_lst(t_env_var **lst)
{
	t_env_var	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->var)
			free((*lst)->var);
		free(*lst);
		*lst = tmp;
	}
}

t_env_var	*get_last_env_var(t_env_var *tmp)
{
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

t_env_var	*new_env_var(char *str)
{
	t_env_var	*new;

	if (!str)
		return (NULL);
	new = malloc(sizeof(t_env_var));
	if (!new)
		return (NULL);
	new->var = str;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

bool	add_env_var(t_env_var **head, char *str)
{
	t_env_var	*new;
	t_env_var	*last;

	new = new_env_var(str);
	if (!new)
		return (free(str), false);
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
	t_env_var	*lst;
	int	i;

	i = 0;
	lst = NULL;
	while (env && env[i])
	{
		if (!add_env_var(&lst, ft_strdup(env[i++])))
			return (free_env_lst(&lst), NULL);
	}
	return (lst);
}
