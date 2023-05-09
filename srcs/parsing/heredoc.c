/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 04:10:58 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/09 23:31:20 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <ms_define.h>
#include <ms_parsing.h>
#include <minishell.h>
#include <stdlib.h>
#include <fcntl.h>
#include <libft.h>
#include <ms_struct.h>
#include <stdio.h>
#include <ms_heredoc.h>

static void	cleanup_heredoc_child(int *tube)
{
	t_fd	dev_null;
	t_fd	tmp_stdin_fileno;

	free(g_ms_params.hd_vars.str);
	free(g_ms_params.hd_vars.limiter);
	my_close(tube[1], tube[0]);
	dev_null = open("/dev/null", O_RDWR);
	tmp_stdin_fileno = dup(g_ms_params.stdin_fileno);
	dup2(dev_null, g_ms_params.stdin_fileno);
	my_close(dev_null, -2);
	gnl_force_finish(1, g_ms_params.stdin_fileno);
	g_ms_params.stdin_fileno = tmp_stdin_fileno;
}

void	heredoc_child(char *limiter, int *tube)
{
	my_close(g_ms_params.input_fd, -2);
	signal(SIGINT, handler_hd_close);
	write(g_ms_params.stdin_fileno, "heredoc> ", 9);
	ms_hd_gnl(g_ms_params.stdin_fileno, &g_ms_params.hd_vars.str);
	g_ms_params.hd_vars.limiter = ft_strjoin(limiter, "\n");
	while (g_ms_params.hd_vars.str && \
	ft_strcmp(g_ms_params.hd_vars.str, g_ms_params.hd_vars.limiter) && !errno)
	{
		write(tube[1], g_ms_params.hd_vars.str, \
		ft_strlen(g_ms_params.hd_vars.str));
		free(g_ms_params.hd_vars.str);
		write(g_ms_params.stdin_fileno, "heredoc> ", 9);
		ms_hd_gnl(g_ms_params.stdin_fileno, &g_ms_params.hd_vars.str);
	}
	if (!g_ms_params.hd_vars.str)
		print_heredoc_syntax_error(limiter);
	cleanup_heredoc_child(tube);
	exit_ms(0, "heredoc");
}

int	heredoc(char *limiter)
{
	int		status;

	if (pipe(g_ms_params.heredoc_pipe) == -1)
		return (-1);
	g_ms_params.heredoc_pid = fork();
	if (g_ms_params.heredoc_pid == -1)
	{
		my_close(g_ms_params.heredoc_pipe[0], g_ms_params.heredoc_pipe[1]);
		return (-1);
	}
	if (!g_ms_params.heredoc_pid)
		heredoc_child(limiter, g_ms_params.heredoc_pipe);
	waitpid(g_ms_params.heredoc_pid, &status, 0);
	g_ms_params.heredoc_pid = 0;
	if (extract_exit_code(status) || errno)
	{
		g_ms_params.heredoc_pid = -3;
		my_close(g_ms_params.heredoc_pipe[0], g_ms_params.heredoc_pipe[1]);
		return (-1);
	}
	my_close(g_ms_params.heredoc_pipe[1], -2);
	return (g_ms_params.heredoc_pipe[0]);
}

void	change_input_fd(t_block *block, t_fd *fd)
{
	if (block->input_source == HEREDOC)
	{
		if (block->io_tab[0] > 2)
			close(block->io_tab[0]);
		block->io_tab[0] = *fd;
	}
	else
	{
		my_close(*fd, -2);
		*fd = INIT_FD_VALUE;
	}
}

int	hd_manager(t_block *block)
{
	t_redirect	*ptr;

	if (!block)
		return (0);
	ptr = block->heredoc;
	while (ptr)
	{
		if (!ptr->hd_lim)
			return (0);
		ptr->joined_name = join_splitted_arg(ptr->hd_lim->next, NULL, false);
		if (errno)
			return (-1);
		ptr->fd = heredoc(ptr->joined_name);
		if (ptr->fd == -1)
			return (-1);
		change_input_fd(block, &ptr->fd);
		ptr = ptr->next;
	}
	if (hd_manager(block->pipe_next) == -1 \
		|| hd_manager(block->next) == -1 \
		|| hd_manager(block->sub) == -1)
		return (-1);
	return (0);
}
