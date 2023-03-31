/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 20:06:53 by amouflet          #+#    #+#             */
/*   Updated: 2023/01/27 19:12:03 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

int	parent_chore(t_task task, int **pfds, int i, int size)
{
	if (i == size)
		get_outfiles(task, pfds[0]);
	if (pipe(pfds[1]))
		return (close(pfds[0][0]), close(pfds[0][0]), \
		f_err_get_cmd(task.cmd), exit(2), 0);
	if (task.cmd->error_str)
	{
		print_error_case(task.cmd);
		return (0);
	}
	return (1);
}

void	print_error_case(t_cmd *cmd)
{
	ft_putstr_fd(cmd->error_str, 2);
	ft_putendl_fd(cmd->cmd[0], 2);
}

void	print_error_output(char *str)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(IS_DIR, 2);
}

void	exec_cmd(t_cmd *cmd, int **pfds, int i, int size)
{
	if (before_exec(pfds[0], i, pfds[1], size))
		if (cmd->cmd && cmd->cmd[0])
		{
			printf("cmd: %s, arg: %s\n", cmd->cmd[0], cmd->cmd[1]);
			execve(cmd->cmd[0], cmd->cmd, NULL);
		}
	f_err_get_cmd(cmd->begin);
	exit(2);
}
