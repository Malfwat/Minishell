/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 22:47:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/06 03:52:36 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <dirent.h>
#include <sys/types.h>
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
	if (!tab)
		return (ft_strsfree(tab), !ft_strcmp(pattern, "*"));
	tmp = str;
	while (tab[i])
	{
		tmp = ft_strstr(tmp, tab[i]);
		if (!tmp)
			return (ft_strsfree(tab), false);
		if (i++ == 0)
			if (*str != *tmp && *pattern != '*')
				return (ft_strsfree(tab), false);
	}
	if (tab[i])
		return (ft_strsfree(tab), false);
	if (pattern[ft_strlen(pattern) - 1] != '*' && ft_strcmp(tmp, tab[i - 1]))
		return (ft_strsfree(tab), false);
	return (ft_strsfree(tab), true);
}

t_arg	*wildcard(char *dir, char *pattern)
{
	DIR				*dirp;
	struct dirent	*dir_entry;
	t_arg			*lst;

	dirp = opendir(dir);
	if (!dirp)
		return (perror("dirp"), NULL);
	errno = 0;
	dir_entry = readdir(dirp);
	while (dir_entry)
	{
		if (compare_wildcard(pattern, dir_entry->d_name))
			ft_addargs(&lst, dir_entry->d_name);
		dir_entry = readdir(dirp);
	}
	if (errno)
		perror("readdir");
	if (closedir(dirp) == -1)
		perror("closedir");
	return (NULL);
}

void	split_path_pattern(char *str, char **path, char **pattern)
{
	char	*tmp;
	int		i;
	int		len;

	i = 0;
	if (!str)
		return ;
	tmp = *path;
	while (str[i + 1])
		i++;
	len = i;
	while (str[i] != '/' && i > 0)
		i--;
	if (i)
	{
		*path = ft_strjoin(tmp, ft_substr(str, 0, i++));
		free(tmp);
	}
	*pattern = ft_substr(str, i, len - i + 1);
}

int	main(int ac, char **av)
{
	char	*path;
	char	*pattern;

	if (ac != 2)
		return (0);
	path = getcwd(NULL, 0);
	pattern = NULL;
	split_path_pattern(av[1], &path, &pattern);
	printf("%s %s \n", path, pattern);
	wildcard(path, pattern);
	return (0);
}
