/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/09 16:45:07 by hateisse         ###   ########.fr       */
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
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void restore_terminal(struct termios saved_term)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_term);
}

// void	free_ms_params(t_minishell ms_params)
// {
	
// }

// void    exit_minishell(t_block *lst, t_env_var *envp_lst, t_minishell ms_params, int exit_value)
// {
//     flood_free(lst);
//     free_env_lst(envp_lst);
// 	(void)ms_params;
//     // free_ms_params(ms_params); // useless pour l'instant car il n'y a que envp qui set free la ligne plus haut
//     // restore_terminal(ms_params.term_params);
//     exit(exit_value);
// }

// exit_minishell se trouve dans term_utils/exit_minishell.c

void	my_dup(t_block *block, int *io_fds)
{
	dup2(block->io_tab[0], 0);
	dup2(block->io_tab[1], 1);
	close(block->io_tab[0]);
	close(block->io_tab[1]);
	if (block->io_tab[0] != io_fds[0])
		close(io_fds[0]);
	if (block->io_tab[1] != io_fds[1])
		close(io_fds[1]);
}

// int	*before_exec(int *fds, int *pipe_fds)
// {
	
// }

// bool	pipex(t_block *block, int *status, t_minishell ms_params, int *io_fds)
// {
// 	int	pipe_fds[2];
// 	int	tmp;
	
// 	tmp = io_fds[0];
// 	while (block)
// 	{
// 		if (block->pipe_next)
// 			pipe(pipe_fds);
// 		if (errno)
// 			return (perror("minishell"), false);
// 		if (!block->pipe_next)
// 			execute_t_block_cmd(block, status, ms_params, (int []){tmp, io_fds[1]});
// 		else
// 			execute_t_block_cmd(block, status, ms_params, (int []){tmp, pipe_fds[1]});
// 		close(tmp);
// 		close(pipe_fds[1]);
// 		dup2(pipe_fds[0], tmp);
// 		close(pipe_fds[0]);
// 		block = block->pipe_next;
// 		if (block && block->io_tab[0] == -1)
// 			block->io_tab[0] = tmp;ad,
// 	}
// 	return (true);
// }

void	ms_perror(char *progname, char *subname, char *error)
{
	if (progname)
	{
		ft_putstr_fd(progname, 2);
		ft_putstr_fd(": ", 2);
	}
	if (subname)
	{
		ft_putstr_fd(subname, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(error, 2);
}

void handle_execve_failure(t_block *block, t_minishell ms_params, char *program_name)
{
	int exit_value;

	exit_value = 2;
	if (errno != ENOENT)
	{
		if (errno == EACCES)
			exit_value = 126; // 126 == command found but cannot be executed
		else if (errno)
			exit_value = 2; // valeur fourre-tout
		ms_perror("minishell", program_name, strerror(errno));
	}
	else
	{
		ms_perror("minishell", program_name, "Command not found");
		exit_value = 127; // 127 == command not found
	}
	exit_minishell(block, ms_params, exit_value);
	// exit_minishell(block, ms_params.envp, ms_params, exit_value);
}


// void	close_block_fds(t_block *block)
// {
// 	if (block->io_tab[0] > 2)
// 	{
// 		close(block->io_tab[0]);
// 		block->io_tab[0] = -2; // valeur quand non initialise
// 	}
// 	if (block->io_tab[1] > 2)
// 	{
// 		block->io_tab[1] = -2;
// 		close(block->io_tab[1]);
// 	}
// }

void	execute_t_block_cmd(t_block *block, t_minishell ms_params)
{
	char	**argv;
	char	**envp;

	(void)status;
	errno = 0;
	argv = build_argv(block->cmd.name, block->cmd.args);
	envp = build_envp(ms_params.envp);
	if (errno)
		return (free(argv), ft_strsfree(envp), perror("minishell"));
	my_dup(block, io_fds);
	block->cmd.pid = fork();
	if (!block->cmd.pid)
	{
		execve(argv[0], argv, envp);
		ft_strsfree(envp);
		free(argv);
		handle_execve_failure(block, ms_params, argv[0]);
	}
	waitpid(block->cmd.pid, &block->cmd.exit_value, 0);
	ft_strsfree(envp);
	free(argv);
}

t_block *find_next_block(t_block *block, bool skip_subshell_parent)
{
	if (block->sub && skip_subshell_parent == true)
		return (find_next_block(block->sub, skip_subshell_parent));
	else if (block->sub && skip_subshell_parent == false)
		return (block->sub);
	else if (block->next)
		return (block->next);
	else if (block->pipe_next)
		return (block->pipe_next);
	return (NULL);
}

t_block	*find_next_executable_block(t_block *block)
{
	int		exit_value;
	int		current_operator;
	t_block	*tmp;

	current_operator = block->operator;
	exit_value = block->cmd.exit_value;
	block = find_next_block(block, false);
	while (block)
	{
		if (block->operator == AND_OPERATOR && exit_value == 0)
			return (find_next_block(block, false));
		else if (block->operator == OR_OPERATOR && exit_value > 0)
			return (find_next_block(block, false));
		else if (block->operator == PIPE_OPERATOR)
			return (find_next_block(block, false));
		else if (block->operator == SEMI_COLON)
			return (find_next_block(block, false));
		else
			block = find_next_block(block, false);
	}
	return (NULL);
}

bool	create_subshell_pipe()

int	execute_cmds(t_block *prev_block, t_block *block, t_minishell ms_params)
{
	t_block	*next_block_to_execute;

	if (!block)
		return (0);
	if (block->do_not_execute)
		return (ft_putstr_fd(block->do_not_execute, 2), close_block_fds(block), false);
	else if (block->subshell_command)
	{
		/*	subshell: si le subshell a un pipe a sa sortie:
		*	1- Changer la sortie du subshell vers un pipe (que si pas de sortie indique)
		*	2- Changer la sortie de chaque cmd dans le subshell vers la sortie de ce pipe
				QUE:
					- S'ils n'ont pas de sortie indique (> >>)
					- Et n'ont pas un operator pipe. ex (echo 1 | echo 2) | cat
						Ici echo 1 n'est pas redirige
						vers le pipe du parent subshell mais vers le pipe de echo 2
						echo 2 lui est redirige vers le pipe du subshell parent
						puis cat prend la sortie du subshell precedent en entree (dup2)
						et travaille avec ca.
		*/
		execute_cmds(block, block->sub, ms_params); // on recursive pour trouver un vrai block
	}
	else
	{
		execute_t_block_cmd(block, ms_params);
		// imaginons : fufhu && echo 2 || echo 3
		// la premiere commande echoue, le prochain block executable sera le echo 3
		// c'est ce block que find_next... est cense renvoye
		next_block_to_execute = find_next_executable_block(block);
		if (!next_block_to_execute)
			return (0); // aucun block executable
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	// char		*res;
	int			type;
	t_block		*head;
	t_minishell	ms_params;

	// t_prompt	prompt_params;

	// set_sig_handler();
	// if (!refresh_prompt_param(&prompt_params))
		// return (1);
	// res = NULL;
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
	ft_strsfree(path);
	if (errno)
		return (exit_minishell(head, ms_params, 0), \
		perror("minishell"), flood_free(head), 0);
	// free(rl_prompt);
	// close(fd);
	return (exit_minishell(head, ms_params, 0), 0);
}
