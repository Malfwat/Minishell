/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 22:47:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:13:59 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <dirent.h>
#include <sys/types.h>
#include <ms_struct.h>
#include <stdio.h>
#include <errno.h>
#include <libft.h>

void	ft_add_t_args(t_args **head, char *str)
{
	t_args	*new;
	t_args	*tmp;

	if (!str)
		return ;
	new = ft_calloc(1, sizeof(*new));
	if (!new)
	{
		free_t_args(*head);
		*head = NULL;
		return ;
	}
	new->final_arg = str;
	if (!(*head))
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}	

bool	open_dir(char *dir, DIR **dirp)
{
	char	*path;

	if (!dir)
		dir = ft_strdup(".");
	else if ((dir)[0] != '/')
	{
		path = getcwd(NULL, 0);
		if (!path)
			return (false);
		dir = ft_strsjoin(3, path, "/", dir);
		free(path);
		if (!dir)
			return (false);
	}
	else
		dir = ft_strdup(dir);
	*dirp = opendir(dir);
	errno = 0;
	free(dir);
	if (!*dirp)
		return (false);
	return (true);
}

t_args	*wildcard(char *dir, char *pattern)
{
	DIR				*dirp;
	struct dirent	*d_entry;
	t_args			*lst;

	lst = NULL;
	if (!open_dir(dir, &dirp))
		return (NULL);
	d_entry = readdir(dirp);
	while (d_entry && !errno)
	{
		if (compare_wildcard(pattern, d_entry->d_name))
		{
			if (dir && ft_strcmp(dir, ".") && d_entry->d_name[0] != '.')
				ft_add_t_args(&lst, ft_strsjoin(3, dir, "/", d_entry->d_name));
			else if (d_entry->d_name[0] != '.')
				ft_add_t_args(&lst, ft_strdup(d_entry->d_name));
		}
		d_entry = readdir(dirp);
	}
	if (errno)
		free_t_args(lst);
	if (closedir(dirp) == -1)
		perror("closedir");
	return (lst);
}

void	split_path_pattern(char *str, char **path, char **pattern)
{
	int		i;
	int		len;

	i = 0;
	if (!str)
		return ;
	while (str[i + 1])
		i++;
	len = i;
	while (str[i] != '/' && i > 0)
		i--;
	if (i)
		*path = ft_substr(str, 0, i++);
	*pattern = ft_substr(str, i, len - i + 1);
}
