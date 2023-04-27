/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:40:10 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/28 00:39:04 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <prompt.h>
#include <libft.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static void	my_exec(char **tab, int *tube)
{
	int	fd_null;

	fd_null = open("/dev/null", O_RDWR);
	if (fd_null == -1 || dup2(fd_null, STDERR_FILENO) == -1)
		perror("minishell");
	else if (close(fd_null) == -1 || close(tube[0]) == -1)
		perror("minishell2");
	else if (dup2(tube[1], STDOUT_FILENO) == -1)
		perror("minishell3");
	else if (execve(tab[0], tab, NULL) == -1)
		perror("minishell4");
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
		return (NULL);
	else if (pid == 0)
		my_exec((char *[]){"/usr/bin/date", "+%T", NULL}, tube);
	else
		waitpid(pid, &status, 0);
	close(tube[1]);
	if (errno)
		return (errno = 0, NULL);
	get_next_line(tube[0], &res);
	gnl_force_finish(1, tube[0]);
	if (*res && res[ft_strlen(res) - 1] == '\n')
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
		return (NULL);
	else if (pid == 0)
		my_exec((char *[]){"/usr/bin/git", "branch", \
		"--show-current", NULL}, tube);
	else
		waitpid(pid, &status, 0);
	close(tube[1]);
	if (errno)
		return (errno = 0, NULL);
	get_next_line(tube[0], &res);
	gnl_force_finish(1, tube[0]);
	if (res && res[ft_strlen(res) - 1] == '\n')
		res[ft_strlen(res) - 1] = 0;
	return (res);
}
