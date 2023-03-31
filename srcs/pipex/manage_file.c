/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 14:46:01 by amouflet          #+#    #+#             */
/*   Updated: 2023/01/25 15:02:10 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

static int	out_file(char *str, int append, t_cmd *cmd)
{
	int	fd;

	if (!append)
		fd = open(str, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	else
		fd = open(str, O_CREAT | O_WRONLY | O_APPEND, 0600);
	if (fd == -1)
		cmd->end->exit_value = 1;
	return (fd);
}

int	get_outfiles(t_task task, int *pipe)
{
	int	in_out;

	in_out = out_file(task.files[1], task.append, task.cmd);
	if (in_out != -1)
	{
		dup2(in_out, pipe[1]);
		close(in_out);
	}
	else
	{
		close(pipe[1]);
		pipe[1] = in_out;
	}
	return (0);
}

static int	in_file(char *str, char *limiter, t_cmd *cmd)
{
	int	fd;

	if (limiter)
		return (here_doc(limiter, cmd));
	fd = open(str, O_RDONLY | O_DIRECTORY);
	if (fd != -1)
		return (print_error_output(str), close(fd), -1);
	fd = open(str, O_RDONLY);
	if (fd == -1)
	{
		if (access(str, F_OK) == 0)
			ft_putstr_fd(NO_PERM, 2);
		else
			ft_putstr_fd(NO_FILE, 2);
		ft_putendl_fd(str, 2);
	}
	return (fd);
}

int	get_infiles(t_task task, int *pipe)
{
	int	in_out;

	in_out = in_file(task.files[0], task.limiter, task.cmd);
	if (in_out != -1)
	{
		dup2(in_out, pipe[0]);
		close(in_out);
	}
	else
	{
		close(pipe[0]);
		pipe[0] = in_out;
	}
	return (0);
}
