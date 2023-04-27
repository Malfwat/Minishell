/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 01:04:05 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/28 00:27:16 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exec_ms.h>
#include <stdio.h>
#include <minishell.h>
#include <sys/wait.h>
#include <env_function.h>

void	child_worker(t_block *blck, t_minishell *ms_params, t_exec_vars exc_vrs)
{
	if (!my_dup(blck))
		return (free_exec_vars(exc_vrs), exit_ms(*ms_params, 2, "exec dup"));
	if (ft_strchr(blck->cmd.args->cmd_w_path, '/'))
		execve(blck->cmd.args->cmd_w_path, exc_vrs.argv, exc_vrs.envp);
	free_exec_vars(exc_vrs);
	handle_execve_failure(*ms_params, blck->cmd.args->final_arg);
}

void	puppet_child(t_block *blck, t_minishell *ms_params, t_exec_vars exc_vrs)
{
	int	*exit_value;

	exit_value = &blck->cmd.exit_value;
	if (blck->cmd.pid)
	{
		waitpid(blck->cmd.pid, exit_value, 0);
		*exit_value = extract_exit_code(*exit_value);
	}
	blck->cmd.pid = fork();
	if (!blck->cmd.pid)
	{
		my_close(blck->io_tab[0], blck->io_tab[1]);
		free_children(&ms_params->children);
		if (blck->pipe_next)
			my_close(blck->pipe_next->io_tab[0], -2);
		free_exec_vars(exc_vrs);
		exit_ms(*ms_params, *exit_value, "puppet_child");
	}
}

bool	cd_implicit(char ***argv, t_exec_vars *vars)
{
	char	**tmp;

	if (ft_strslen(*argv) != 1)
		return (false);
	if (is_directory(**argv) <= 0)
		return (errno = 0, false);
	tmp = ft_calloc(3, sizeof(char *));
	if (!tmp)
		return (false);
	tmp[0] = ft_strdup("cd");
	tmp[1] = **argv;
	free(*argv);
	*argv = tmp;
	vars->cd_implicit = tmp[0];
	return (true);
}

void	launch_cmd(t_block *block, t_minishell *ms_params)
{
	t_exec_vars	vars;

	vars = init_exec_vars(*ms_params, block);
	if (is_builtin(vars.argv[0]) || cd_implicit(&vars.argv, &vars))
		exec_builtin(block, ms_params, vars);
	else
		block->cmd.pid = fork();
	if (errno)
		return ;
	if (block->cmd.pid == 0 && !is_builtin(vars.argv[0]) && !errno)
		child_worker(block, ms_params, vars);
	else if (is_builtin(vars.argv[0]) && !errno)
		puppet_child(block, ms_params, vars);
	free_exec_vars(vars);
}
