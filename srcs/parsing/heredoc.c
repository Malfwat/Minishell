/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 04:10:58 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/28 04:50:40 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <ms_define.h>
#include <parsing_ms.h>
#include <minishell.h>
#include <stdlib.h>
#include <libft.h>
#include <struct_ms.h>

void	handler_close(int num, siginfo_t *info, void *context)
{
	t_hd_vars	*hd_vars;

	hd_vars = (t_hd_vars *)context;
	(void)num;
	(void)info;
	my_close(hd_vars->ms_params->heredoc_pipe[0], \
	hd_vars->ms_params->heredoc_pipe[1]);
	free(hd_vars->str);
	free(hd_vars->tmp);
	exit_ms(*hd_vars->ms_params, 0, "handler_close");
}

void	heredoc_child(char *limiter, t_minishell *ms_params, int *tube)
{
	struct sigaction	sa;
	t_hd_vars			hd_vars;

	ft_bzero(&hd_vars, sizeof(hd_vars));
	sa.sa_sigaction = &handler_close;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, (void *)&hd_vars);
	get_next_line(0, &hd_vars.str);
	hd_vars.tmp = ft_strjoin(limiter, "\n");
	while (hd_vars.str && ft_strcmp(hd_vars.str, hd_vars.tmp) && !errno)
	{
		write(tube[1], hd_vars.str, ft_strlen(hd_vars.str));
		free(hd_vars.str);
		write(ms_params->stdin_fileno, "> ", 2);
		get_next_line(0, &hd_vars.str);
	}
	if (!hd_vars.str)
		print_heredoc_syntax_error(limiter);
	free(hd_vars.str);
	free(hd_vars.tmp);
	my_close(tube[1], tube[0]);
	exit_ms(*ms_params, 0, "heredoc");
}

int	heredoc(char *limiter, t_minishell *ms_params)
{
	int		status;

	if (pipe(ms_params->heredoc_pipe) == -1)
		return (-1);
	write(ms_params->stdin_fileno, "> ", 2);
	signal(SIGINT, &handler_heredoc);
	ms_params->heredoc_pid = fork();
	if (!ms_params->heredoc_pid)
		heredoc_child(limiter, ms_params, ms_params->heredoc_pipe);
	waitpid(ms_params->heredoc_pid, &status, 0);
	ms_params->heredoc_pid = 0;
	signal(SIGINT, &handler_func);
	if (WIFSIGNALED(status))
		return (ms_params->heredoc_pid = -3, \
		my_close(ms_params->heredoc_pipe[0], ms_params->heredoc_pipe[1]), -1);
	my_close(ms_params->heredoc_pipe[1], -2);
	return (ms_params->heredoc_pipe[0]);
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

int	hd_manager(t_block *block, t_minishell *ms_params)
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
		ptr->fd = heredoc(ptr->joined_name, ms_params);
		if (ptr->fd == -1)
			return (-1);
		change_input_fd(block, &ptr->fd);
		ptr = ptr->next;
	}
	if (hd_manager(block->pipe_next, ms_params) == -1 \
		|| hd_manager(block->next, ms_params) == -1 \
		|| hd_manager(block->sub, ms_params) == -1)
		return (-1);
	return (0);
}
