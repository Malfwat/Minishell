/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 11:42:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/13 21:17:55 by hateisse         ###   ########.fr       */
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

char	**build_envp(t_env_var	*envp)
{
	char		**tab;
	int			len;
	t_env_var	*tmp;

	tmp = envp;
	len = 0;
	while (envp)
	{
		envp = envp->next;
		len++;
	}
	tab = ft_calloc((len + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	len = 0;
	while (tmp)
	{
		tab[len] = ft_strsjoin(3, tmp->var_name, "=", tmp->var_value);
		if (!tab[len++])
			return (ft_strsfree(tab), NULL);
		tmp = tmp->next;
	}
	return (tab);
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
