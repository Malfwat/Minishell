/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:33:59 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/25 21:01:57 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <env_function.h>
#include <errno.h>
#include <minishell.h>
#include <string.h>
#include <libft.h>
#include <struct_ms.h>

void	cd(t_minishell *ms_params, char **tab, t_fd fd)
{
	char	*dir;
	char	*tmp;
	char	*cwd;
	char	*previous_directory;

	if (tab[0] && tab[1])
		return (ms_perror("minishell", "cd", "too many arguments"));
	if (!*tab)
		dir = getenv("HOME");
	else if (!ft_strcmp(*tab, "-"))
	{
		dir = ms_params->previous_directory;
		if (fd == INIT_FD_VALUE)
			fd = 1;
		ft_putendl_fd(ms_params->previous_directory, fd);
	}
	else
		dir =  *tab;
	previous_directory = getcwd(NULL, 0);
	if (chdir(dir) == -1 || !previous_directory)
	{
		if (previous_directory)
			ms_perror("minishell", "cd", strerror(errno));
		errno = 0;
		free(previous_directory);
		ms_params->last_exit_code = 1;
		return ;
	}
	free(ms_params->previous_directory);
	ms_params->previous_directory = previous_directory;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	tmp = ft_strjoin("PWD=", cwd);
	if (!tmp)
		return (free(cwd));
	export(ms_params, (char *[]){tmp, NULL}, 0, 1);
	free(cwd);
	free(tmp);
	return ;
}
