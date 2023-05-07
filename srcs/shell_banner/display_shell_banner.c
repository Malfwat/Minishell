/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_shell_banner.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 00:54:18 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/08 01:56:02 by hateisse         ###   ########.fr       */
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

void	kill_banner_processes(int sig)
{
	pid_t	pid;
	int		status;

	(void)sig;
	pid = fork();
	if (pid == -1)
		exit_ms(2, "kill_banner: fatal:");
	if (!pid)
	{
		chdir("srcs/shell_banner");
		execve("/usr/bin/pkill", (char *[]){"/usr/bin/pkill", \
		"minishell_banner", NULL}, NULL);
		exit(0);
	}
	waitpid(pid, &status, 0);
}

void	exec_shell_banner(void)
{
	pid_t	pid;
	int		status;
	char	**envp;

	pid = fork();
	if (pid == -1)
		return ;
	if (!pid)
	{
		// mute_fd(STDERR_FILENO);
		dup2(g_ms_params.stdin_fileno, 0);
		dup2(g_ms_params.stdin_fileno, 1);
		signal(SIGINT, &kill_banner_processes);
		envp = build_envp(g_ms_params.envp);
		chdir("srcs/shell_banner");
		execve("./minishell_banner.sh", \
		(char *[]){"./minishell_banner.sh", "-T", NULL}, envp);
		ms_perror("minishell", "exec_shell_banner", strerror(errno));
		exit(1);
	}
	waitpid(pid, &status, 0);
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
	errno = 0;
}
