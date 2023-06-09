/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 07:53:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/07 14:08:28 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <minishell.h>
#include <stdbool.h>
#include <errno.h>
#include <libft.h>

static bool	check_access_and_set_path(char *path, char *cmd, char **dest)
{
	*dest = ft_strsjoin(3, path, "/", cmd);
	if (errno)
		return (false);
	if (access(*dest, F_OK) == 0)
		return (true);
	free(*dest);
	*dest = NULL;
	return (true);
}

bool	get_cmd_path(char **path, char **cmd, char **dest)
{
	int		i;

	i = 0;
	if (ft_strchr(*cmd, '/') || is_builtin(*cmd) || !**cmd)
	{
		*dest = ft_strdup(*cmd);
		return (true);
	}
	while (path && path[i])
	{
		if (!check_access_and_set_path(path[i++], *cmd, dest))
			return (false);
		if (*dest)
			return (true);
		errno = 0;
	}
	*dest = ft_strdup(*cmd);
	return (true);
}
