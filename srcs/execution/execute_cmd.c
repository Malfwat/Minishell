/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 04:49:46 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 04:17:31 by malfwa           ###   ########.fr       */
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
	execve(exc_vrs.argv[0], exc_vrs.argv, exc_vrs.envp);
	free_exec_vars(exc_vrs);
	handle_execve_failure(*ms_params, blck->cmd.args->final_arg);
}

void	execute_t_block_cmd(t_block *block, t_minishell *ms_params)
{
	t_exec_vars	exec_vars;

	errno = 0;
	if (!init_exec_io(block, ms_params))
		return ;
	exec_vars = init_exec_vars(*ms_params, block);
	if (is_builtin(exec_vars.argv[0]))
		return (exec_builtin(block, ms_params, exec_vars));
	block->cmd.pid = fork();
	if (block->cmd.pid == 0)
		child_worker(block, ms_params, exec_vars);
	if (block->io_tab[0] >= 0)
		close(block->io_tab[0]);
	if (block->io_tab[1] >= 0)
		close(block->io_tab[1]);
	free_exec_vars(exec_vars);
	if (block->cmd.pid == -1 || errno)
		exit_ms(*ms_params, 2, "exec fork");
	if (block->operator == AND_OPERATOR || block->operator == OR_OPERATOR
		|| block->operator == SEMI_COLON)
	{
		if (waitpid(block->cmd.pid, &block->cmd.exit_value, 0) == -1)
			exit_ms(*ms_params, 2, "waitpid");
		ms_params->last_exit_code = block->cmd.exit_value;
		free(find_env_var(ms_params->envp, "?")->var_value);
		find_env_var(ms_params->envp, "?")->var_value = ft_itoa(block->cmd.exit_value);
	}
	else
		store_pid(block->cmd.pid, &ms_params->children);
}

pid_t	create_subshell(t_block *block, t_minishell *ms_prm)
{
	pid_t	sub_pid;

	sub_pid = fork();
	if (!sub_pid)
	{
		free_children(&ms_prm->children);
		my_dup(block);
		execute_commands(block->sub, ms_prm);
		exit_ms(*ms_prm, extract_exit_code(wait_children(ms_prm)), "subshell");
	}
	if (block->io_tab[0] != INIT_FD_VALUE)
	{
		if (close(block->io_tab[0]) == -1)
		{
			if (block->io_tab[1] != INIT_FD_VALUE)
				close(block->io_tab[1]);
			exit_ms(*ms_prm, 2, "subshell");
		}
	}
	if (block->io_tab[1] != INIT_FD_VALUE)
	{
		if (close(block->io_tab[1]) == -1)
			exit_ms(*ms_prm, 2, "subshell");
	}
	return (close_sub_fds(block->sub), sub_pid);
}

void	manage_subshell(t_block *block, t_minishell *ms_params)
{
	if (block->pipe_next)
		create_pipe(block);
	block->cmd.pid = create_subshell(block, ms_params);
	if (block->operator == AND_OPERATOR || block->operator == OR_OPERATOR
		|| block->operator == SEMI_COLON)
	{
		if (waitpid(block->cmd.pid, &block->cmd.exit_value, 0) == -1)
			exit_ms(*ms_params, 2, "waitpid");
		ms_params->last_exit_code = block->cmd.exit_value;
	}
	else
		store_pid(block->cmd.pid, &ms_params->children);
}

int	execute_commands(t_block *block, t_minishell *ms_params)
{
	t_block	*next_block_to_execute;

	if (!block)
		return (0);
	else if (block->subshell_command)
		manage_subshell(block, ms_params);
	else
	{
		if (block->operator == PIPE_OPERATOR)
			create_pipe(block);
		execute_t_block_cmd(block, ms_params);
	}
	next_block_to_execute = find_next_executable_block(block);
	if (!next_block_to_execute)
		return (0);
	else
		execute_commands(next_block_to_execute, ms_params);
	return (0);
}
