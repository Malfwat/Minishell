/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 11:42:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/07 13:11:00 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ms_env_function.h>
#include <stdlib.h>

char	*get_env_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] != '=')
		return (NULL);
	return (ft_substr(str, 0, i));
}

char	*get_env_value(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] != '=' || str[i + 1] == '\0')
		return (ft_strdup(""));
	return (ft_substr(str, i + 1, ft_strlen(&str[i + 1])));
}

t_env	*find_env_var(t_env	*envp, char *str)
{
	while (envp)
	{
		if (!ft_strcmp(str, envp->var_name))
			return (envp);
		envp = envp->next;
	}
	return (envp);
}

