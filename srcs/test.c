/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/08 17:02:25 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <prompt.h>
#include <env_function.h>
#include <signal_ms.h>
#include <parsing_ms.h>
#include <sys/stat.h>
#include <minishell.h>
#include <history.h>


void restore_terminal(struct termios saved_term)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_term);
}

// void	free_ms_params(t_minishell ms_params)
// {
	
// }

void    exit_minishell(t_block *lst, t_env_var *envp_lst, t_minishell ms_params, int exit_value)
{
    flood_free(lst);
    free_t_env(envp_lst);
	(void)ms_params;
    // free_ms_params(ms_params); // useless pour l'instant car il n'y a que envp qui set free la ligne plus haut
    // restore_terminal(ms_params.term_params);
    exit(exit_value);
}

void	my_dup(int *io_fds)
{
	dup2(io_fds[0], 0);
	dup2(io_fds[1], 1);
	close(io_fds[0]);
	close(io_fds[1]);
}

int	*before_exec(int *fds, int *pipe_fds)
{
	
}

bool	pipex(t_block *block, int *status, t_minishell ms_params, int *io_fds)
{
	int	pipe_fds[2];
	int	tmp;
	
	tmp = io_fds[0];
	while (block)
	{
		if (block->pipe_next)
			pipe(pipe_fds);
		if (errno)
			return (perror("minishell"), false);
		if (!block->pipe_next)
			execute_t_block_cmd(block, status, ms_params, (int []){tmp, io_fds[1]});
		else
			execute_t_block_cmd(block, status, ms_params, (int []){tmp, pipe_fds[1]});
		close(tmp);
		close(pipe_fds[1]);
		dup2(pipe_fds[0], tmp);
		close(pipe_fds[0]);
		block = block->pipe_next;
	}
	return (true);
}

void	execute_t_block_cmd(t_block *block, int *status, t_minishell ms_params, int *io_fds)
{
	char **argv;
	char **envp;
	int exit_value;

	(void)status;
	errno = 0;
	argv = build_argv(block->cmd.name, block->cmd.args);
	envp = build_envp(ms_params.envp);
	my_dup(io_fds);
	if (errno)
		return (free(argv), free(envp), perror("minishell"));
	block->cmd.pid = fork();
	if (!block->cmd.pid)
	{
		errno = 0;
		execve(argv[0], argv, envp);
		// exit(0);
		exit_value = 127;
		if (!access(argv[0], F_OK))
		{
			perror(argv[0]);
			exit_value = 126;
		}
		else
			printf("%s: Command not found\n", argv[0]);
			
		return (free(envp), free(argv), \
		exit_minishell(block, ms_params.envp, ms_params, exit_value));
	}
	free(envp);
	free(argv);
}

// int	execute_cmds(int *status, t_block *block, int *pipe, t_minishell ms_params)
// {
// 	if (block->do_not_execute)
// 		return (ft_putstr_fd(block->do_not_execute, 2), false);
// 	else if (block->subshell_command)
// 		execute_cmds(status, block, pipe, ms_params);
// 	else
// 		pipex()
// 	// 	while (block->pipe_next)
// 	// 	{
// 	// 		execute_t_block_cmd(block, status, ms_params.envp, ms_params);
// 	// 		// pipex
// 	// 	}
// 	// 	if (block->operator == AND_OPERATOR && *status != 0)
// 	// 		return (0);
// 	// 	else
// 	// 	{
			
// 	// 	}
// 	// }
// 	return (0);
// }

int	main(int ac, char **av, char **env)
{
	char		*res;
	int			type;
	t_block		*head;
	t_minishell	ms_params;
	// t_prompt	prompt_params;

	// set_sig_handler();
	// if (!refresh_prompt_param(&prompt_params))
		// return (1);
	res = NULL;
	type = -1;
	ms_params.envp = get_env_var(env);
	(void)ac;
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	// int fd = get_my_history();
	// if (fd == -1)
		// return (2);
	// readline(build_prompt(prompt_params));
	// my_add_history(rl_line_buffer, fd);
	char *str =  ft_strdup(av[1]);
	head = new_block();
	char **path = ft_split(getenv("PATH"), ':');
	if (!path)
		return (0);
	parse_cmds(&head, str, path);
	io_manager(head);
	execute_t_block_cmd(head, &type, ms_params, head->io_tab);
		// execute_cmds(head, );
	if (errno)
		return (perror("minishell"), flood_free(head), 0);
	exit_minishell(head, ms_params.envp, ms_params, 0);
	// free(rl_prompt);
	// close(fd);
	printf("[%d] res: '%s'\n", type, res);
}
