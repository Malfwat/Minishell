/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_shell_banner.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 00:54:18 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/09 15:11:29 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_signal.h>
#include <signal.h>
#include <minishell.h>
#include <unistd.h>
#include <sys/types.h>
#include <ms_define.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <ms_exec.h>
#include <stdio.h>
#include <sys/ioctl.h>

void	kill_banner_processes(int sig)
{
	(void)sig;
	killpg(g_ms_params.banner_gpid, SIGTERM);
	printf("\033[2J\033[H");
}

pid_t	exec_shell_banner(void)
{
	pid_t	pid;
	char	**envp;

	pid = fork();
	if (pid == -1)
		return (-1);
	else if (!pid)
	{
		if (setpgid(0, 0) == -1)
			exit_ms(2, "exec_shell_banner");
		dup2(g_ms_params.stdin_fileno, 1);
		my_close(g_ms_params.stdin_fileno, -2);
		signal(SIGINT, &do_nothing);
		envp = build_envp(g_ms_params.envp);
		if (!chdir("./srcs/shell_banner"))
			execve("./ms_banner.sh", (char *[]){"./ms_banner.sh", NULL}, envp);
		ft_strsfree(envp);
		exit_ms(2, "exec_shell_banner");
	}
	else
	{
		g_ms_params.banner_gpid = pid;
		signal(SIGINT, &kill_banner_processes);
	}
	return (pid);
}

bool	mute_fd(t_fd fd)
{
	t_fd	dev_null_fd;

	dev_null_fd = open("/dev/null", O_RDWR);
	if (dev_null_fd == -1)
		return (false);
	else if (dup2(dev_null_fd, fd) == -1)
		return (close(dev_null_fd), false);
	else if (close(dev_null_fd) == -1)
		return (false);
	return (true);
}

void	display_shell_banner(void)
{
	pid_t	exec_banner_pid;
	int		status;

	exec_banner_pid = exec_shell_banner();
	if (exec_banner_pid != -1)
	{
		waitpid(exec_banner_pid, &status, 0);
		printf("\033[2J\033[H\033[?25h\n");
	}
	signal(SIGINT, SIG_DFL);
	errno = 0;
}
