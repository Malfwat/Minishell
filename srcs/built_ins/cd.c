/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:33:59 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/01 09:08:01 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <env_function.h>
#include <errno.h>
#include <minishell.h>
#include <string.h>
#include <libft.h>
#include <struct_ms.h>

void	error_case(int *exit_code, char *previous_directory)
{
	if (previous_directory)
	{
		ms_perror("minishell", "cd", strerror(errno));
		*exit_code = 1;
	}
	errno = 0;
	free(previous_directory);
	return ;
}

void	change_dir(char *dir)
{
	char	*tmp;
	char	*cwd;
	char	*previous_directory;

	previous_directory = getcwd(NULL, 0);
	if (chdir(dir) == -1 || !previous_directory)
		return (error_case(&g_ms_params.last_exit_code, previous_directory));
	free(g_ms_params.previous_directory);
	g_ms_params.previous_directory = previous_directory;
	cwd = getcwd(NULL, 0);
	tmp = ft_strjoin("PWD=", cwd);
	if (!errno)
		export((char *[]){tmp, NULL}, 0, 1);
	return (free(tmp), free(cwd));
}

void	cd(char **tab, t_fd fd)
{
	char	*dir;

	if (tab[0] && tab[1])
		return (ms_perror("minishell", "cd", "too many arguments"));
	if (!*tab)
		dir = getenv("HOME");
	else if (!ft_strcmp(*tab, "-"))
	{
		dir = g_ms_params.previous_directory;
		if (fd == INIT_FD_VALUE)
			fd = 1;
		ft_putendl_fd(g_ms_params.previous_directory, fd);
	}
	else
		dir = *tab;
	change_dir(dir);
}
