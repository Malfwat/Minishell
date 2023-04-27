/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 01:04:05 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 01:43:54 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exec_ms.h>
#include <minishell.h>
#include <sys/wait.h>
#include <env_function.h>

void	child_worker(t_block *blck, t_minishell *ms_params, t_exec_vars exc_vrs)
{
	if (!my_dup(blck))
		return (free_exec_vars(exc_vrs), exit_ms(*ms_params, 2, "exec dup"));
	execve(blck->cmd.args->cmd_w_path, exc_vrs.argv, exc_vrs.envp);
	if (blck->cmd.args->cmd_w_path != exc_vrs.argv[0])
		free(blck->cmd.args->cmd_w_path);
	free_exec_vars(exc_vrs);
	handle_execve_failure(*ms_params, blck->cmd.args->final_arg);
}

void	puppet_child(t_block *blck, t_minishell *ms_params, t_exec_vars exc_vrs)
{
	int	*exit_value;

	exit_value = &blck->cmd.exit_value;
	my_close(blck->io_tab[0], blck->io_tab[1]);
	if (blck->cmd.args->cmd_w_path != exc_vrs.argv[0])
		free(blck->cmd.args->cmd_w_path);
	free_exec_vars(exc_vrs);
	if (blck->pipe_next)
		close(blck->pipe_next->io_tab[0]);
	if (blck->cmd.pid)
	{
		waitpid(blck->cmd.pid, exit_value, 0);
		*exit_value = extract_exit_code(*exit_value);
	}
	exit_ms(*ms_params, *exit_value, "puppet_child");
}

void	launch_cmd(t_block *block, t_minishell *ms_params)
{
	t_exec_vars	vars;

	vars = init_exec_vars(*ms_params, block);
	if (is_builtin(vars.argv[0]))
		exec_builtin(block, ms_params, vars);
	block->cmd.pid = fork();
	if (block->cmd.pid == 0 && !is_builtin(vars.argv[0]) && !errno)
		child_worker(block, ms_params, vars);
	else if (block->cmd.pid == 0 && is_builtin(vars.argv[0]) && !errno)
		puppet_child(block, ms_params, vars);
	if (block->cmd.args->cmd_w_path != vars.argv[0])
		free(block->cmd.args->cmd_w_path);
	free_exec_vars(vars);
}
