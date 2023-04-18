/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:31:37 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/18 23:32:12 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <dirent.h>
#include <sys/types.h>
#include <struct_ms.h>
#include <stdio.h>
#include <errno.h>
#include <libft.h>

bool	compare_wildcard(char *pattern, char *str)
{
	int		i;
	char	**tab;
	char	*tmp;

	i = 0;
	tab = ft_split(pattern, '*');
	if (!tab || !*tab)
		return (ft_strsfree(tab), !errno);
	tmp = str;
	while (tab[i])
	{
		tmp = ft_strstr(tmp, tab[i]);
		if (!tmp)
			return (ft_strsfree(tab), false);
		if (i == 0)
			if (*pattern != '*' && *str != *tmp)
				return (ft_strsfree(tab), false);
		tmp += ft_strlen(tab[i++]);
	}
	if (tab[i])
		return (ft_strsfree(tab), false);
	if (pattern[ft_strlen(pattern) - 1] != '*' && *tmp)
		return (ft_strsfree(tab), false);
	return (ft_strsfree(tab), true);
}

bool	manage_wildcard(t_args **head, char *str)
{
	char	*path;
	char	*pattern;

	path = NULL;
	pattern = NULL;
	split_path_pattern(str, &path, &pattern);
	*head = wildcard(path, pattern);
	free(pattern);
	free(path);
	return (true);
}
