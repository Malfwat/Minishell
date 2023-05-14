/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:26:27 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/14 22:01:08 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_define.h>
#include <fcntl.h>
#include <libft.h>
#include <errno.h>
#include <stdarg.h>
#include <minishell.h>
#include <stdlib.h>
#include <stdio.h>

void	handler_hd_close(int num)
{
	t_fd	tmp;
	t_fd	dev_null;

	dev_null = open("/dev/null", O_RDWR);
	(void)num;
	my_close(g_ms_params.heredoc_pipe[0], g_ms_params.heredoc_pipe[1]);
	free(g_ms_params.hd_vars.limiter);
	free(g_ms_params.hd_vars.str);
	tmp = dup(g_ms_params.stdin_fileno);
	dup2(dev_null, g_ms_params.stdin_fileno);
	close(dev_null);
	gnl_force_finish(1, g_ms_params.stdin_fileno);
	g_ms_params.stdin_fileno = tmp;
	exit_ms(2, "handler_close");
}

void	handler_readline(int num)
{
	(void)num;
	errno = 0;
	my_close(g_ms_params.readline_pipe[1], -2);
	write(g_ms_params.stdin_fileno, "\n", 1);
	free(g_ms_params.ms_prompt);
	g_ms_params.ms_prompt = NULL;
	exit_ms(130, "handler readline");
}

void	child_reset_signals(int nb, ...)
{
	va_list	arg;
	int		signum;

	va_start(arg, nb);
	while (nb--)
	{
		signum = va_arg(arg, int);
		signal(signum, SIG_DFL);
	}
	va_end(arg);
	restore_terminal_params();
}
