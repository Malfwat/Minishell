/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 11:42:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 21:27:23 by hateisse         ###   ########.fr       */
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
	tab = malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (NULL);
	tab[len] = NULL;
	len = 0;
	while (tmp)
	{
		tab[len] = tmp->var;
		len++;
		tmp = tmp->next;
	}
	return (tab);
}

t_env_var	*find_env_var(t_env_var	*lst, char *str)
{
	char	*tmp;

	while (lst)
	{
		tmp = get_env_var_name(lst->var);
		if (ft_strcmp(str, tmp) == 0)
		{
			free(tmp);
			return (lst);
		}
		free(tmp);
		lst = lst->next;
	}
	return (lst);
}
