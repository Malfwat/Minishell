/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_ins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 21:09:00 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/27 01:39:51 by malfwa           ###   ########.fr       */
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

void	launch_builtins(t_minishell *ms_params, t_exec_vars vars, t_fd fd[2])
{
	char	*str;

	str = vars.argv[0];
	if (!ft_strcmp(str, "env"))
		env(ms_params->envp, fd[1]);
	else if (!ft_strcmp(str, "pwd"))
		pwd(fd[1]);
	else if (!ft_strcmp(str, "unset"))
		unset(&ms_params->envp, &vars.argv[1]);
	else if (!ft_strcmp(str, "export"))
		export(ms_params, &vars.argv[1], 0, fd[1]);
	else if (!ft_strcmp(str, "cd"))
		cd(ms_params, &vars.argv[1], fd[1]);
	else if (!ft_strcmp(str, "echo"))
		ms_echo(&vars.argv[1], fd[1]);
	else if (!ft_strcmp(str, "exit"))
		ms_exit_builtin(ms_params, vars, fd);
}

int	is_pipe(int fd)
{
	struct stat	filestat;

	if (fd < 0)
		return (-1);
	if (fstat(fd, &filestat) < 0)
		return (-1);
	return (S_ISFIFO(filestat.st_mode));
}

void	my_close(t_fd a, t_fd b)
{
	if (a >= 0)
		close(a);
	if (b >= 0)
		close(b);
}

void	exec_builtin(t_block *block, t_minishell *ms_params, t_exec_vars vars)
{
	pid_t	pid;

	if (is_pipe(block->io_tab[0]) > 0 || is_pipe(block->io_tab[1]) > 0)
	{
		pid = fork();
		if (pid == -1)
			return ;
		if (!pid)
		{
			launch_builtins(ms_params, vars, block->io_tab);
			my_close(block->io_tab[0], block->io_tab[1]);
			if (block->pipe_next)
				close(block->pipe_next->io_tab[0]);
			free_exec_vars(vars);
			exit_ms(*ms_params, block->cmd.exit_value, "builtin fork");
		}
		block->cmd.pid = pid;
	}
	else
		launch_builtins(ms_params, vars, block->io_tab);
	block->cmd.exit_value = ms_params->last_exit_code;
	free(find_env_var(ms_params->envp, "?")->var_value);
	find_env_var(ms_params->envp, "?")->var_value \
	= ft_itoa(extract_exit_code(block->cmd.exit_value));
}
