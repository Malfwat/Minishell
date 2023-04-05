/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 22:47:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/06 01:14:10 by malfwa           ###   ########.fr       */
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
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (ft_strcmp(pattern, "*") == 0)
		return (true);
	if (!ft_strstr(str, pattern))
		return (false);
	return (true);
}

t_arg	*wildcard(char *dir, char *pattern)
{
	DIR				*dirp;
	struct dirent	*dir_entry;

	// On ouvre un flux vers le repertoire courant
	dirp = opendir(dir);
	if (!dirp)
	{
		perror("dirp");
		return (NULL);
	}
	// On accede aux element du repertoire, un par un
	// on set errno a 0 pour distinguer une erreur de la fin de la liste
	errno = 0;
	dir_entry = readdir(dirp);
	while (dir_entry)
	{
		if (compare_wildcard(pattern, dir_entry->d_name))
			printf("Nom du fichier: %s\n", dir_entry->d_name);
		dir_entry = readdir(dirp);
	}
	if (errno)
	{
		perror("readdir");
	}
	// On ferme le flux vers le repertoire pointe par dirp
	if (closedir(dirp) == -1)
	{
		perror("closedir");
		return (NULL);
	}
}

void	split_path_pattern(char *str, char **path, char **pattern)
{
	char	*tmp;
	int 	i;
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

int main(int ac, char **av)
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