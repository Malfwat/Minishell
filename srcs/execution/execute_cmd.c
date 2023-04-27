/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 04:49:46 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 21:20:32 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exec_ms.h>
#include <minishell.h>
#include <sys/wait.h>
#include <env_function.h>

void	wait_before_next_pipe_line(t_block *block, t_minishell *ms_params)
{
	if (block->cmd.pid && waitpid(block->cmd.pid, \
	&block->cmd.exit_value, 0) == -1)
		exit_ms(*ms_params, 2, "waitpid");
	ms_params->last_exit_code = block->cmd.exit_value;
	free(find_env_var(ms_params->envp, "?")->var_value);
	find_env_var(ms_params->envp, "?")->var_value \
	= ft_itoa(block->cmd.exit_value);
}

void	execute_t_block_cmd(t_block *block, t_minishell *ms_params)
{
	errno = 0;
	if (!init_exec_io(block, ms_params))
		return (my_close(block->io_tab[0], block->io_tab[1]));
	if (block->cmd.args)
		launch_cmd(block, ms_params);
	my_close(block->io_tab[0], block->io_tab[1]);
	if (block->cmd.pid == -1 || errno)
	{
		if (block->pipe_next)
			my_close(block->pipe_next->io_tab[0], -2);
		exit_ms(*ms_params, 2, "exec fork");
	}
	if (block->operator == AND_OPERATOR || block->operator == OR_OPERATOR
		|| block->operator == SEMI_COLON)
		wait_before_next_pipe_line(block, ms_params);
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
				my_close(block->io_tab[1], -2);
			exit_ms(*ms_prm, 2, "subshell");
		}
	}
	if (block->io_tab[1] > 2)
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
