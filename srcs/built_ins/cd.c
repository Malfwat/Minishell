/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:33:59 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 03:14:39 by hateisse         ###   ########.fr       */
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

bool	change_dir(char *dir)
{
	char	*tmp;
	char	*tmp_oldpwd;
	char	*cwd;
	char	*previous_directory;

	previous_directory = getcwd(NULL, 0);
	if (chdir(dir) == -1 || !previous_directory)
		return (error_case(&g_ms_params.last_exit_code, previous_directory), false);
	free(g_ms_params.previous_directory);
	g_ms_params.previous_directory = previous_directory;
	cwd = getcwd(NULL, 0);
	tmp = ft_strjoin("PWD=", cwd);
	tmp_oldpwd = ft_strjoin("OLDPWD=", previous_directory);
	if (!errno)
	{
		export((char *[]){tmp, NULL}, PUBLIC_VAR, 1);
		export((char *[]){tmp_oldpwd, NULL}, INTERNAL_VAR, 1);
	}
	return (free(tmp), free(cwd), free(tmp_oldpwd), true);
}

bool	cd(char **tab, t_fd fd)
{
	char	*dir;
	t_env	*home;

	if (tab[0] && tab[1])
		return (ms_perror("minishell", "cd", "too many arguments"), false);
	if (!*tab)
	{
		home = find_env_var(g_ms_params.envp, "HOME");
		if (!home)
			return (ms_perror("minishell", "cd", "HOME not set"), false);
		dir = home->var_value;
	}
	else if (!ft_strcmp(*tab, "-"))
	{
		dir = g_ms_params.previous_directory;
		if (!dir)
			return (ms_perror("minishell", "cd", "OLDPWD not set"), false);
		if (fd == INIT_FD_VALUE)
			fd = 1;
		ft_putendl_fd(g_ms_params.previous_directory, fd);
	}
	else
		dir = *tab;
	return (change_dir(dir));
}
