/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 11:42:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/18 05:34:29 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <env_function.h>
#include <stdlib.h>

char	*get_env_var_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
		i++;
	return (ft_substr(str, 0, i));
}

char	*get_env_var_value(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
		i++;
	return (ft_substr(str, i + 1, ft_strlen(&str[i + 1])));
}

t_env_var	*find_env_var(t_env_var	*envp, char *str)
{
	while (envp)
	{
		if (!ft_strcmp(str, envp->var_name))
			return (envp);
		envp = envp->next;
	}
	return (envp);
}

void	update_env_var(t_env_var **head)
{
	t_env_var	*tmp;
	t_env_var	*curr;

	curr = *head;
	while (curr)
	{
		tmp = curr;
		curr = curr->next;
		if (tmp->temp)
			unset(head, tmp);
	}
}
