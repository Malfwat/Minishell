/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:33:59 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 02:48:03 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <env_function.h>
#include <errno.h>
#include <minishell.h>
#include <string.h>
#include <libft.h>
#include <struct_ms.h>

void	cd(t_minishell *ms_params, t_env_var *head, char **tab)
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
		dir = ms_params->previous_directory;
	else
		dir =  *tab;
	previous_directory = getcwd(NULL, 0);
	if (chdir(dir) == -1 || !previous_directory)
	{
		ms_perror("minishell", "cd", strerror(errno));
		errno = 0;
		free(previous_directory);
		ms_params->last_exit_code = 256;
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
	export(&head, (char *[]){tmp, NULL}, 0);
	free(cwd);
	return ;
}
