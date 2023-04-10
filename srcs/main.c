/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/10 20:54:09 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <ms_define.h>
#include <signal_ms.h>
#include <parsing_ms.h>
#include <prompt.h>
#include <env_function.h>
#include <libft.h>
#include <sys/stat.h>
#include <history.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void	ms_perror(char *program, char *subname, char *error)
{
	if (program)
	{
		ft_putstr_fd(program, 2);
		ft_putstr_fd(": ", 2);
	}
	if (subname)
	{
		ft_putstr_fd(subname, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(error, 2);
}

void handle_execve_failure(t_minishell ms_params, char *program_name)
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
	errno = 0;
	exit_ms(ms_params, exit_value, NULL);
	// exit_ms(ms_params.envp, ms_params, exit_value);
}


void	close_block_fds(t_block *block)
{
	if (block->io_tab[0] > 2)
	{
		close(block->io_tab[0]);
		block->io_tab[0] = -2; // valeur quand non initialise
	}
	if (block->io_tab[1] > 2)
	{
		block->io_tab[1] = -2;
		close(block->io_tab[1]);
	}
}
#include <stdio.h>

bool	my_dup(t_block *block)
{
	if (block->io_tab[0] != INIT_FD_VALUE)
	{
		if (dup2(block->io_tab[0], 0) == -1 || close(block->io_tab[0]) == -1) 
			return (false);
		//dprintf(2,"(dup to %d)open:%d\n", 0, block->io_tab[0]);
	}
	if (block->io_tab[1] != INIT_FD_VALUE)
	{
		if (dup2(block->io_tab[1], 1) == -1 || close(block->io_tab[1]) == -1)
			return (false);
		//(2,"(dup to %d)open:%d\n", 1, block->io_tab[1]);
	}
	if (block->pipe_next)
	{
		close(block->pipe_next->io_tab[0]);
		//dprintf(2,"close:%d\n", block->pipe_next->io_tab[0]);
	}
	return (true);
}

void	execute_t_block_cmd(t_block *block, t_minishell *ms_params)
{
	char	**argv;
	char	**envp;

	errno = 0;
	argv = build_argv(block->cmd.name, block->cmd.args);
	envp = build_envp(ms_params->envp);
	if (errno)
		return (free(argv), ft_strsfree(envp), exit_ms(*ms_params, 2, "exec"));
	block->cmd.pid = fork();
	if (!block->cmd.pid)
	{
		if (errno || !my_dup(block))
			return (free(argv), ft_strsfree(envp), exit_ms(*ms_params, 2, "exec"));
		execve(argv[0], argv, envp);
		ft_strsfree(envp);
		free(argv);
		// if (!isatty(0))
			// close(0);
		if (block->io_tab[0] >= 0)
			close(block->io_tab[0]);
		if (block->io_tab[1] >= 0)
			close(block->io_tab[1]);
		if (block->pipe_next)
			close(block->pipe_next->io_tab[0]);
		handle_execve_failure(*ms_params, block->cmd.name);
	}
	if (block->io_tab[0] >= 0)
		close(block->io_tab[0]);
	if (block->io_tab[1] >= 0)
		close(block->io_tab[1]);
	ft_strsfree(envp);
	free(argv);
	if (block->cmd.pid == -1)
		exit_ms(*ms_params, 2, "exec");
	store_pid(block->cmd.pid, &ms_params->children);
	// waitpid(block->cmd.pid, &block->cmd.exit_value, 0);
}

t_block *find_next_block(t_block *block, bool ignore_sub)
{
	if (block->sub && !ignore_sub)
		return (block->sub);
	else if (block->pipe_next)
		return (block->pipe_next);
	else if (block->next)
		return (block->next);
	return (NULL);
}

t_block	*find_next_executable_block(t_block *block)
{
	int		exit_value;
	int		current_operator;

	current_operator = block->operator;
	exit_value = block->cmd.exit_value;
	while (block)
	{
		if (block->operator == AND_OPERATOR && exit_value == SUCCESS)
			return (find_next_block(block, true));
		else if (block->operator == OR_OPERATOR && exit_value > 0)
			return (find_next_block(block, true));
		else if (block->operator == PIPE_OPERATOR)
			return (find_next_block(block, true));
		else if (block->operator == SEMI_COLON)
			return (find_next_block(block, true));
		else
			block = find_next_block(block, true);
	}
	return (NULL);
}

bool	create_pipe(t_block *block)
{
	t_block	*next_block;
	int		tube[2];

	next_block = find_next_block(block, true);
	if (pipe(tube) == -1)
		return (false);
	if (next_block->io_tab[0] == INIT_FD_VALUE || next_block->io_is_overwritable[0])
	{
		// check if fd is lost
		next_block->io_tab[0] = tube[0];
		next_block->io_is_overwritable[0] = true; // Penser a init a false la ou on creer le block
	}
	if (block->io_tab[1] == INIT_FD_VALUE || !next_block->io_is_overwritable[1])
	{
		block->io_tab[1] = tube[1];
		block->io_is_overwritable[1] = true;
	}
	return (true);
}

void	infanticides(t_pids *preys)
{
	while (preys)
	{
		kill(preys->pid, SIGTERM);
		preys = preys->next;
	}
}

bool	store_pid(pid_t pid, t_pids **nursery)
{
	t_pids	*new;
	t_pids	*lst;

	new = ft_calloc(1, sizeof(t_pids));
	if (!new)
		return (infanticides(*nursery), false);
	new->pid = pid;
	if (!*nursery)
	{
		*nursery = new;
		return (true);
	}
	else
	{
		lst = *nursery;
		while (lst->next)
			lst = lst->next;
		lst->next = new;
	}
	return (true);
}

void	free_children(t_pids *children)
{
	t_pids	*tmp;
	
	while (children)
	{
		tmp = children->next;
		free(children);
		children = tmp;
	}
}

int	wait_children(t_pids *children)
{
	int	status;

	while (children)
	{
		waitpid(children->pid, &status, 0);
		children = children->next;
	}
	return (status);
}

pid_t	create_subshell(t_block *block, t_minishell *ms_params)
{
	pid_t	sub_pid;
	// int		i1;
	// int		i2;

	sub_pid = fork();
	if (!sub_pid)
	{
		free_children(ms_params->children);
		ms_params->children = NULL;
		my_dup(block);
		execute_cmds(block->sub, ms_params); // on passe au contenu du subshell immediatement
		exit_ms(*ms_params, wait_children(ms_params->children), NULL);
	}
	if (block->io_tab[0] != INIT_FD_VALUE && close(block->io_tab[0]) == -1)
	{
		if (block->io_tab[1] != INIT_FD_VALUE)
			close(block->io_tab[1]);
		exit_ms(*ms_params, 2, "subshell");
	}
	if (block->io_tab[1] != INIT_FD_VALUE && close(block->io_tab[1]) == -1)
		exit_ms(*ms_params, 2, "subshell");
	return (sub_pid);
}

int	execute_cmds(t_block *block, t_minishell *ms_params)
{
	t_block	*next_block_to_execute;
	pid_t	sub_pid;
	// int		status

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
			Meme chose pour pour le cas d'une entree
			Penser a gerer que le pipe dans un subshell prevaut sur le pipe en dehors
		*/
		/*
		*	Penser au fait que le fd du pipe du subshell sera donne a ses enfants
		*	et qu'il faudra eviter de double-close un meme fd
		*/
		if (block->pipe_next)
			create_pipe(block);
		sub_pid = create_subshell(block, ms_params);
		store_pid(sub_pid, &ms_params->children);
		// waitpid(sub_pid, &status, 0);
	}
	else
	{
		if (block->operator == PIPE_OPERATOR)
			create_pipe(block);
		execute_t_block_cmd(block, ms_params);
		// imaginons : fufhu && echo 2 || echo 3
		// la premiere commande echoue, le prochain block executable sera le echo 3
		// c'est ce block que find_next... est cense renvoye
		next_block_to_execute = find_next_executable_block(block);
		if (!next_block_to_execute)
			return (0); // aucun block executable
		else
			execute_cmds(next_block_to_execute, ms_params);
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	// char		*res;
	int			type;
	t_block		*head;
	t_minishell	ms_params;

	ft_memset(&ms_params, 0, sizeof(t_minishell));
	save_terminal_params(&ms_params);
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
		return (perror(""), 0);
	parse_cmds(&head, str, path);
	ft_strsfree(path);
	if (errno)
		return (exit_ms(ms_params, 2, "parsing"), 0);
	io_manager(head);
	if (errno)
		ms_perror("minishell", "io_manager", strerror(errno));
	ms_params.head = head;
	execute_cmds(head, &ms_params);
	//execute_t_block_cmd(head, &type, ms_params, head->io_tab);
		// execute_cmds(head, );
	// free(rl_prompt);
	// close(fd);
	// ft_putnbr_fd(isatty(ms_params.stdin_fileno), 36);
	wait_children(ms_params.children);
	return (exit_ms(ms_params, 0, NULL), 0);
}
