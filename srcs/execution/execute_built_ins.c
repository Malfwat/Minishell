/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_ins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 21:09:00 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/03 22:12:07 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <struct_ms.h>
#include <env_function.h>
#include <exec_ms.h>
#include <ms_define.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

bool	is_builtin(char *str)
{
	if (!ft_strcmp(str, "env"))
		return (true);
	if (!ft_strcmp(str, "pwd"))
		return (true);
	if (!ft_strcmp(str, "unset"))
		return (true);
	if (!ft_strcmp(str, "export"))
		return (true);
	if (!ft_strcmp(str, "cd"))
		return (true);
	if (!ft_strcmp(str, "echo"))
		return (true);
	if (!ft_strcmp(str, "exit"))
		return (true);
	return (false);
}

void	launch_builtins(t_exec_vars vars, t_fd fd[2])
{
	char	*str;

	str = vars.argv[0];
	if (!ft_strcmp(str, "env"))
		env(g_ms_params.envp, fd[1]);
	else if (!ft_strcmp(str, "pwd"))
		pwd(fd[1]);
	else if (!ft_strcmp(str, "unset"))
		unset(&g_ms_params.envp, &vars.argv[1]);
	else if (!ft_strcmp(str, "export"))
		export(&vars.argv[1], 0, fd[1]);
	else if (!ft_strcmp(str, "cd"))
		cd(&vars.argv[1], fd[1]);
	else if (!ft_strcmp(str, "echo"))
		ms_echo(&vars.argv[1], fd[1]);
	else if (!ft_strcmp(str, "exit"))
		ms_exit_builtin(vars, fd);
}

int	is_pipe(int fd)
{
	struct stat	filestat;

	if (fd < 0)
		return (-1);
	if (fstat(fd, &filestat) < 0)
		return (errno = 0, -1);
	return (S_ISFIFO(filestat.st_mode));
}

void	my_close(t_fd a, t_fd b)
{
	if (a > 2)
		close(a);
	if (b > 2)
		close(b);
}

void	exec_builtin(t_block *block, t_exec_vars vars)
{
	pid_t	pid;

	if (is_pipe(block->io_tab[0]) > 0 || is_pipe(block->io_tab[1]) > 0)
	{
		pid = fork();
		if (pid == -1)
			return ;
		if (!pid)
		{
			signal(SIGINT, SIG_DFL);
			my_close(g_ms_params.input_fd, -2);
			free_children(&g_ms_params.children);
			launch_builtins(vars, block->io_tab);
			my_close(block->io_tab[0], block->io_tab[1]);
			if (block->pipe_next)
				my_close(block->pipe_next->io_tab[0], -2);
			free_exec_vars(vars);
			exit_ms(block->cmd.exit_value, "builtin fork");
		}
		block->cmd.pid = pid;
	}
	else
		launch_builtins(vars, block->io_tab);
	block->cmd.exit_value = g_ms_params.last_exit_code;
	// set_env_exit_var(extract_exit_code(block->cmd.exit_value));
}
