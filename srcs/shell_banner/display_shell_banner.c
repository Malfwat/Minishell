/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_shell_banner.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 00:54:18 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/08 07:31:47 by hateisse         ###   ########.fr       */
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
	printf("\033[2J\033[H\n");
}

void	exec_shell_banner(void)
{
	pid_t	pid;
	int		status;
	char	**envp;

	signal(SIGINT, &kill_banner_processes);
	pid = fork();
	if (pid == -1)
		return ;
	if (!pid)
	{
		setpgid(0, 0);
		// mute_fd(STDERR_FILENO);
		dup2(g_ms_params.stdin_fileno, 1);
		signal(SIGINT, &do_nothing);
		envp = build_envp(g_ms_params.envp);
		if (!chdir("./srcs/shell_banner"))
			execve("./ms_banner.sh", \
			(char *[]){"./ms_banner.sh", NULL}, envp);
		exit_ms(2, "exec_shell_banner");
	}
	else
		g_ms_params.banner_gpid = pid;
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
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
	signal(SIGINT, do_nothing);
	exec_shell_banner();
	signal(SIGINT, SIG_DFL);
	printf("\033[2J\033[H\n");
	errno = 0;
}
