/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 23:06:36 by malfwa            #+#    #+#             */
/*   Updated: 2023/01/25 19:28:45 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

int	check_exe(t_cmd *cmd)
{
	if (cmd->cmd[0] && access(cmd->cmd[0], X_OK) == 0)
		return (1);
	else if (cmd->cmd[0] && access(cmd->cmd[0], F_OK) == 0)
	{
		cmd->exit_value = 126;
		cmd->error_str = NO_PERM;
		return (0);
	}
	cmd->exit_value = 127;
	cmd->error_str = NO_CMD;
	return (0);
}

static int	check_access(char *path, t_cmd *cmd)
{
	char	*str;
	char	*tmp;

	str = ft_strjoin(path, cmd->cmd[0]);
	if (str && access(str, X_OK) == 0)
	{
		tmp = cmd->cmd[0];
		cmd->cmd[0] = str;
		return (free(tmp), 1);
	}
	else if (str && access(str, F_OK) == 0)
	{
		cmd->exit_value = 126;
		cmd->error_str = NO_PERM;
	}
	free(str);
	return (0);
}

int	check_cmd(char **path, t_cmd *cmd)
{
	int		i;

	i = 0;
	if (cmd && cmd->cmd && cmd->cmd[0] && ((cmd->cmd[0][0] == '.' \
	&& cmd->cmd[0][1] == '/') || cmd->cmd[0][0] == '/'))
		return (check_exe(cmd));
	while (cmd->cmd && cmd->cmd[0] && path && path[i])
	{
		if (check_access(path[i], cmd))
			return (1);
		i++;
	}
	cmd->exit_value = 127;
	cmd->error_str = NO_CMD;
	return (0);
}
