/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:40:10 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/14 11:52:55 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_prompt.h>
#include <libft.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <minishell.h>

static void	my_exec(char **tab, int *tube)
{
	int	fd_null;

	fd_null = open("/dev/null", O_RDWR);
	if (fd_null == -1 || dup2(fd_null, STDERR_FILENO) == -1)
		perror("minishell");
	else if (close(fd_null) == -1 || close(tube[0]) == -1)
		perror("minishell");
	else if (dup2(tube[1], STDOUT_FILENO) == -1)
		perror("minishell");
	else if (close(tube[1]) == -1)
		perror("minishell");
	execve(tab[0], tab, NULL);
	errno = 0;
	exit_ms(1, "prompt");
}

char	*fetch_current_time(void)
{
	int		status;
	int		tube[2];
	int		pid;
	char	*res;

	if (pipe(tube))
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (my_close(tube[0], tube[1]), NULL);
	else if (pid == 0)
		my_exec((char *[]){"/usr/bin/date", "+%T", NULL}, tube);
	else
		waitpid(pid, &status, 0);
	my_close(tube[1], -2);
	if (errno || extract_exit_code(status) != 0)
		return (my_close(tube[0], -2), errno = 0, NULL);
	get_next_line(tube[0], &res);
	gnl_force_finish(1, tube[0]);
	if (res && *res && res[ft_strlen(res) - 1] == '\n')
		res[ft_strlen(res) - 1] = 0;
	return (res);
}

char	*fetch_git_cwd_branch_name(void)
{
	int		status;
	int		tube[2];
	int		pid;
	char	*res;

	if (pipe(tube))
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (my_close(tube[0], tube[1]), NULL);
	else if (pid == 0)
		my_exec((char *[]){"/usr/bin/git", "branch", \
		"--show-current", NULL}, tube);
	else
		waitpid(pid, &status, 0);
	my_close(tube[1], -2);
	if (errno || extract_exit_code(status) != 0)
		return (my_close(tube[0], -2), errno = 0, NULL);
	get_next_line(tube[0], &res);
	gnl_force_finish(1, tube[0]);
	if (res && *res && res[ft_strlen(res) - 1] == '\n')
		res[ft_strlen(res) - 1] = 0;
	return (res);
}
