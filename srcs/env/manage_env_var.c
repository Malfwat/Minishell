/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 11:42:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/06 05:20:31 by malfwa           ###   ########.fr       */
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

char	**t_env_var_to_array(t_env_var	*lst)
{
	char		**tab;
	int			len;
	t_env_var	*tmp;

	tmp = lst;
	len = 0;
	while (lst)
	{
		lst = lst->next;
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

t_env_var	*find_env_var(t_env_var	*lst, char *str)
{
	while (lst)
	{
		if (!ft_strcmp(str, lst->var_name))
			return (lst);
		lst = lst->next;
	}
	return (lst);
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