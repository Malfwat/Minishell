/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_ins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 21:09:00 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/11 19:48:15 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <ms_struct.h>
#include <ms_env_function.h>
#include <ms_exec.h>
#include <ms_signal.h>
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
	if (!ft_strcmp(str, "export") || ft_strchr(str, '='))
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
	int		exit_value;

	str = vars.argv[0];
	exit_value = 0;
	if (!ft_strcmp(str, "env"))
		exit_value = env(g_ms_params.envp, fd[1]);
	else if (!ft_strcmp(str, "pwd"))
		exit_value = pwd(fd[1]);
	else if (!ft_strcmp(str, "unset"))
		exit_value = unset(&g_ms_params.envp, &vars.argv[1]);
	else if (!ft_strcmp(str, "export"))
		exit_value = export(&vars.argv[1], PUBLIC_VAR, fd[1]);
	else if (ft_strchr(str, '='))
		exit_value = export(vars.argv, INTERNAL_VAR, fd[1]);
	else if (!ft_strcmp(str, "cd"))
		exit_value = cd(&vars.argv[1], fd[1]);
	else if (!ft_strcmp(str, "echo"))
		exit_value = ms_echo(&vars.argv[1], fd[1]);
	else if (!ft_strcmp(str, "exit"))
		exit_value = ms_exit_builtin(vars, fd);
	else
		return ;
	g_ms_params.last_exit_code = !exit_value;
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

	if ((is_pipe(block->io_tab[0]) > 0 || is_pipe(block->io_tab[1]) > 0) \
		&& ft_strcmp(block->cmd.args->final_arg, "exit"))
	{
		pid = fork();
		if (pid == -1)
			return ;
		if (!pid)
		{
			child_reset_signals(2, SIGINT, SIGQUIT);
			my_close(g_ms_params.input_fd, -2);
			free_children(&g_ms_params.children);
			launch_builtins(vars, block->io_tab);
			my_close(block->io_tab[0], block->io_tab[1]);
			if (block->pipe_next)
				my_close(block->pipe_next->io_tab[0], -2);
			free_exec_vars(vars);
			exit_ms(g_ms_params.last_exit_code, "builtin fork");
		}
		block->cmd.pid = pid;
	}
	else
		launch_builtins(vars, block->io_tab);
	block->cmd.exit_value = g_ms_params.last_exit_code;
}
