/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 22:47:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/12 14:39:46 by malfwa           ###   ########.fr       */
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

t_arg	*wildcard(char *dir, char *pattern)
{
	DIR				*dirp;
	struct dirent	*dir_entry;
	t_arg			*lst;

	printf("dir: %s\npattern: %s\n", dir, pattern);

	dirp = opendir(dir);
	if (!dirp)
		return (perror("dirp"), NULL);
	dir_entry = readdir(dirp);
	lst = NULL;
	while (dir_entry)
	{
		if (compare_wildcard(pattern, dir_entry->d_name))
			ft_addargs(&lst, ft_strdup(dir_entry->d_name));
		dir_entry = readdir(dirp);
	}
	if (errno)
		perror("readdir");
	if (closedir(dirp) == -1)
		perror("closedir");
	return (lst);
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
		if ((*path)[0] != '/')
			*path = ft_strjoin(tmp, ft_substr(str, 0, i++));
		else
			*path = ft_substr(str, 0, i++);
		free(tmp);
	}
	*pattern = ft_substr(str, i, len - i + 1);
}

bool	manage_wildcard(t_arg **head, char *str)
{
	char	*path;
	char	*pattern;

	path = getcwd(NULL, 0);
	if (!path)
		return (false);
	pattern = NULL;
	split_path_pattern(str, &path, &pattern);
	*head = wildcard(path, pattern);
	return (true);
}
