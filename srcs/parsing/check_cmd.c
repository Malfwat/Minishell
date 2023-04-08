/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 07:53:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/08 08:28:54 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <libft.h>

static int	check_access(char *path, char **cmd)
{
	char	*str;
	char	*tmp;

	str = ft_strsjoin(3, path, "/", *cmd);
	if (str && access(str, F_OK) == 0)
	{
		tmp = *cmd;
		*cmd = str;
		return (free(tmp), 1);
	}
	free(str);
	return (0);
}

char	*check_cmd(char **path, char *cmd)
{
	int		i;

	i = 0;
	if (!ft_strncmp(cmd, "./", 2) || *cmd == '/')
		return (cmd);
	while (path && path[i])
		if (check_access(path[i++], &cmd))
			break ;
	return (cmd);
}
