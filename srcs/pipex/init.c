/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 18:16:28 by amouflet          #+#    #+#             */
/*   Updated: 2023/01/25 19:31:41 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <stdio.h>

void	add_slash(char **paths)
{
	int		i;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(tmp);
		i++;
	}
}

char	**get_paths(char **env)
{
	char	**paths;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp("PATH=", env[i], 5))
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	if (!paths)
		exit(2);
	add_slash(paths);
	return (paths);
}

int	get_cmd(int ac, char **av, char **env, t_cmd **cmd)
{
	int		i;
	char	**path;

	i = 0;
	path = get_paths(env);
	while (i < ac - 1)
	{
		if (add_cmd(cmd, ft_split(av[i], ' ')))
			return (1);
		check_cmd(path, (*cmd)->end);
		i++;
	}
	if (path)
		free_tab(path);
	return (0);
}

t_task	init_t_stack(int ac, char **av)
{
	t_task	task;

	task.cmd = NULL;
	task.append = 0;
	task.limiter = NULL;
	task.files[0] = av[1];
	task.files[1] = av[ac - 1];
	return (task);
}
