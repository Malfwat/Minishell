/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 17:42:27 by amouflet          #+#    #+#             */
/*   Updated: 2023/03/31 17:15:35 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pipex.h>

static void	my_dup(int i, int *pp, int *fd, int size)
{
	if (fd[0] != -1)
		dup2(fd[0], 0);
	if (i == size && fd[1] != -1)
	{
		dup2(fd[1], 1);
		dup2(fd[1], 2);
	}
	else if (pp[1] != -1)
	{
		dup2(pp[1], 1);
		dup2(pp[1], 2);
	}
}

int	before_exec(int *in_out, int i, int *pfds, int size)
{
	my_dup(i, pfds, in_out, size);
	if (in_out[0] != -1)
		if (close(in_out[0]) == -1)
			ft_putstr_fd("error close\n", 2);
	if (in_out[1] != -1)
		if (close(in_out[1]) == -1)
			ft_putstr_fd("error close\n", 2);
	if (pfds[0] != -1)
		if (close(pfds[0]) == -1)
			ft_putstr_fd("error close\n", 2);
	if (pfds[1] != -1)
		if (close(pfds[1]) == -1)
			ft_putstr_fd("error close\n", 2);
	if (i == size && in_out[1] == -1)
		return (0);
	if (in_out[0] == -1)
		return (0);
	return (1);
}

static int	change_wrong_fd(int *pipe0, t_cmd **cmd)
{
	int	tmp;

	tmp = dup(pipe0[1]);
	close(pipe0[1]);
	if (pipe(pipe0))
		return (f_err_get_cmd(*cmd), close(tmp), 1);
	if (dup2(tmp, pipe0[1]) == -1)
		return (f_err_get_cmd(*cmd), close(tmp), close(pipe0[0]), 1);
	close(tmp);
	return (0);
}

static int	after_exec(int *pipe1, int *pipe0, int i, t_cmd **cmd)
{
	if (pipe0[0] == -1)
	{
		if (change_wrong_fd(pipe0, cmd))
			return (close(pipe1[0]), close(pipe1[1]), exit(0), 1);
	}
	close(pipe0[0]);
	if (pipe1[1] != -1)
		close(pipe1[1]);
	if (pipe1[0] != -1)
	{
		dup2(pipe1[0], pipe0[0]);
		close(pipe1[0]);
	}
	(*cmd) = (*cmd)->next;
	return (i + 1);
}

void	child(int *pfds[2], t_task task, int size)
{
	int		i;
	t_task tmp;

	i = 0;
	get_infiles(task, pfds[0]);
	tmp = task;
	while (task.cmd)
	{
		if (parent_chore(task, pfds, i, size))
		{
			if (!(i == 0 && pfds[0][0] == -1) \
			&& (!task.cmd->next || !task.cmd->next->error_str))
				task.cmd->pid = fork();
			if (task.cmd->pid == -1)
				exit(EXIT_FAILURE);
			if (task.cmd->pid == 0)
				exec_cmd(task.cmd, pfds, i, size);
		}
		i = after_exec(pfds[1], pfds[0], i, &task.cmd);
	}
	while (tmp.cmd)
	{
		waitpid(tmp.cmd->pid, NULL, 0);
		tmp.cmd = tmp.cmd->next;
	}
	return (ending(task.files[1], pfds));
}
