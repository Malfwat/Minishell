/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/08 07:12:52 by malfwa           ###   ########.fr       */
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

int	main(int ac, char **av)
{
	char		*res;
	int			type;
	t_block		*head;
	// t_prompt	prompt_params;

	// set_sig_handler();
	// if (!refresh_prompt_param(&prompt_params))
		// return (1);
	res = NULL;
	type = -1;
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

	parse_cmds(&head, str);
		// execute_cmds(head, );
	if (errno)
		return (perror("minishell"), flood_free(head), 0);
	flood_free(head);
	// free(rl_prompt);
	// close(fd);
	printf("[%d] res: '%s'\n", type, res);
}


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
    // free_ms_params(ms_params); // useless pour l'instant car il n'y a que envp qui set free la ligne plus haut
    restore_terminal(ms_params.term_params);
    exit(exit_value);
}


void	execute_t_block_cmd(t_block *block, int *status, t_minishell ms_params)
{
	char **argv;
	char **envp;
	int exit_value;

	(void)status;
	argv = build_argv(block->cmd.name, block->cmd.args);
	envp = build_envp(ms_params.envp);
	if (!argv || !envp)
		return (free(argv), free(envp), perror("minishell"));
	block->cmd.pid = fork();
	if (!block->cmd.pid)
	{
		execve(argv[0], argv, envp);
		perror(argv[0]);
		exit_value = 127;
		if (!access(argv[0], F_OK))
			exit_value = 126;
		return (ft_strsfree(envp), ft_strsfree(argv), \
		exit_minishell(block, ms_params.envp, ms_params, exit_value));
	}
	free(envp);
	free(argv);
}

int	execute_cmds(int *status, t_block *block, int *pipe, t_minishell ms_params)
{
	if (block->do_not_execute)
		return (ft_putstr_fd(block->do_not_execute, 2), false);
	else if (block->subshell_command)
		execute_cmds(status, block, pipe, ms_params);
	else
		pipex()
	// 	while (block->pipe_next)
	// 	{
	// 		execute_t_block_cmd(block, status, ms_params.envp, ms_params);
	// 		// pipex
	// 	}
	// 	if (block->operator == AND_OPERATOR && *status != 0)
	// 		return (0);
	// 	else
	// 	{
			
	// 	}
	// }
	return (0);
}