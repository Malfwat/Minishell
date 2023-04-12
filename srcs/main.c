/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/12 15:13:15 by malfwa           ###   ########.fr       */
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
#include <ncurses.h>
#include <term.h>

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

bool	my_dup(t_block *block)
{
	if (block->io_tab[0] != INIT_FD_VALUE)
	{
		if (dup2(block->io_tab[0], 0) == -1 || close(block->io_tab[0]) == -1) 
			return (false);
		dprintf(2, "[%d]dup:%d\n", getpid(), block->io_tab[0]);
		dprintf(2, "[%d]clsose:%d\n", getpid(), block->io_tab[0]);
	}
	if (block->io_tab[1] != INIT_FD_VALUE)
	{
		int d, c;

		d = dup2(block->io_tab[1], 1);
		c = close(block->io_tab[1]);
		if (d == -1 || c == -1)
		{
			dprintf(2, "[%d]dup:%d\n", getpid(), block->io_tab[1]);
			dprintf(2, "is errno ? %d\nc = %i, d = %i \n", errno, c, d);
			dprintf(2, "[%d]cldose:%d\n", getpid(), block->io_tab[1]);
			return (false);
		}
	}
	if (block->pipe_next)
	{
		close(block->pipe_next->io_tab[0]);
		dprintf(2, "[%d]cflose:%d\n", getpid(), block->pipe_next->io_tab[0]);
	}
	return (true);
}

void	close_sub_fds(t_block *head)
{
	while (head)
	{
		if (head->io_tab[1] != INIT_FD_VALUE)
			close(head->io_tab[1]);
		if (head->io_tab[0] != INIT_FD_VALUE)
			close(head->io_tab[0]);
		head = head->next;
	}
}

void	execute_t_block_cmd(t_block *block, t_minishell *ms_params)
{
	char	**argv;
	char	**envp;

	errno = 0;
	argv = build_argv(block->cmd.name, &block->cmd.args);
	envp = build_envp(ms_params->envp);
	if (errno)
		return (free(argv), ft_strsfree(envp), exit_ms(*ms_params, 2, "exec 0"));
	block->cmd.pid = fork();
	if (!block->cmd.pid)
	{
		if (errno || !my_dup(block))
		{
			dprintf(2, "is errno after my_dup ? %d\n", errno);
			dprintf(2, "[%i] io_tab[%i][%i]", getpid(), block->io_tab[0], block->io_tab[1]);
			return (free(argv), ft_strsfree(envp), exit_ms(*ms_params, 2, "exec 1"));
		}
		execve(argv[0], argv, envp);
		ft_strsfree(envp);
		free(argv);
		// if (!isatty(0))
			// close(0);
		if (block->io_tab[0] >= 0)
		{
			if (close(block->io_tab[0]) != -1)
				dprintf(2, "[%d]close:%d\n", getpid(), block->io_tab[0]);
		}
		if (block->io_tab[1] >= 0)
		{
			if (close(block->io_tab[1]) != -1)
				dprintf(2, "[%d]close:%d\n", getpid(), block->io_tab[1]);
		}
		if (block->pipe_next)
		{
			if (close(block->pipe_next->io_tab[0]) != -1)
				dprintf(2, "[%d]close:%d\n", getpid(), block->pipe_next->io_tab[0]);
		}
		handle_execve_failure(*ms_params, block->cmd.name);
	}
	if (block->io_tab[0] >= 0)
	{
		if (close(block->io_tab[0]) != -1)
			dprintf(2, "[%d]close:%d\n", getpid(), block->io_tab[0]);
		else
			dprintf(2, "??? 2\n");
	}
	if (block->io_tab[1] >= 0)
	{
		if (close(block->io_tab[1]) != -1)
			dprintf(2, "[%d]close:%d\n", getpid(), block->io_tab[1]);
		else
			dprintf(2, "%s ??? 1 %i\n", block->cmd.name, block->io_tab[1]);
	}
	ft_strsfree(envp);
	free(argv);
	if (block->cmd.pid == -1)
		exit_ms(*ms_params, 2, "exec 2");
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
	dprintf(2, "[%i] %d pipe %d\n", getpid(), tube[0], tube[1]);
	if (next_block->io_tab[0] == INIT_FD_VALUE || next_block->io_is_overwritable[0])
	{
		// check if fd is lost
		if (next_block->io_tab[0] != INIT_FD_VALUE)
			close(next_block->io_tab[0]);
		next_block->io_tab[0] = tube[0];
		next_block->io_is_overwritable[0] = true; // Penser a init a false la ou on creer le block
	}
	else
		close(tube[0]);
	if (block->io_tab[1] == INIT_FD_VALUE || !block->io_is_overwritable[1])
	{
		if (block->io_tab[1] != INIT_FD_VALUE)
			close(block->io_tab[1]);
		block->io_tab[1] = tube[1];
		block->io_is_overwritable[1] = true;
	}
	else
		close(tube[1]);
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

void	free_children(t_pids **children)
{
	t_pids	*tmp;
	t_pids	*ptr;

	ptr = *children;
	while (ptr)
	{
		tmp = ptr->next;
		free(ptr);
		ptr = tmp;
	}
	*children = NULL;
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
		
		free_children(&ms_params->children);
		ms_params->children = NULL;
		my_dup(block);
		execute_cmds(block->sub, ms_params); // on passe au contenu du subshell immediatement
		exit_ms(*ms_params, wait_children(ms_params->children), NULL);
	}
	close_sub_fds(block->sub);
	if (block->io_tab[0] != INIT_FD_VALUE)
	{
		if (close(block->io_tab[0]) == -1)
		{
			if (block->io_tab[1] != INIT_FD_VALUE)
			{
				close(block->io_tab[1]);
				dprintf(2, "[%d]close:%d\n", getpid(), block->io_tab[1]);
			}
			exit_ms(*ms_params, 2, "subshell");
		}
		dprintf(2, "[%d]close:%d\n", getpid(), block->io_tab[1]);
	}
	if (block->io_tab[1] != INIT_FD_VALUE)
	{
		if (close(block->io_tab[1]) == -1)
			exit_ms(*ms_params, 2, "subshell");
		dprintf(2, "[%d]close:%d\n", getpid(), block->io_tab[1]);
	}
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
		{
			create_pipe(block);
			dprintf(2, "pid create_pipe %i\n", getpid());
		}
		sub_pid = create_subshell(block, ms_params);
		store_pid(sub_pid, &ms_params->children);
		// waitpid(sub_pid, &status, 0);
	}
	else
	{
		if (block->operator == PIPE_OPERATOR)
			create_pipe(block);
		execute_t_block_cmd(block, ms_params);
	}
		// imaginons : fufhu && echo 2 || echo 3
		// la premiere commande echoue, le prochain block executable sera le echo 3
		// c'est ce block que find_next... est cense renvoye
	next_block_to_execute = find_next_executable_block(block);
	if (!next_block_to_execute)
		return (0); // aucun block executable
	else
		execute_cmds(next_block_to_execute, ms_params);
	return (0);
}

int	get_cursor_position(void)
{
	struct termios	term;
	struct termios	restore;
	char			*sc_cursor_pos;
	char			buf[100];
	int				i;
	int				ret;
	char			*digit_index[1];

	ft_bzero(buf, 10);
	sc_cursor_pos = tgetstr("u7", NULL);
	if (!sc_cursor_pos)
		return (-1);
	tcgetattr(0, &term);
	tcgetattr(0, &restore);
	term.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(0, TCSANOW, &term);
	write(1, sc_cursor_pos, ft_strlen(sc_cursor_pos));
	ret = 1;
	i = 0;
	while (ret > 0 && i < 9)
	{
		ret = read(STDIN_FILENO, &buf[i], 1);
		if (ret <= 0 || buf[i] == 'R')
			break ;
		i++;
	}
	digit_index[0] = ft_strchr(buf, ';');
	tcsetattr(0, TCSANOW, &restore);
	if (!digit_index[0])
		return (-1);
	return (ft_atoi(digit_index[0] + 1));
}

void	ensure_prompt_position(void)
{
	int	x;

	x = get_cursor_position();
	if (x == -1)
		return ;
	else if (x == 1)
		return ;
	else
		ft_putstr_fd("\033[47m\033[30m%\033[0m\n", STDOUT_FILENO);
}

int	main(int ac, char **av, char **env)
{
	// char		*res;
	char		*tmp;
	char		*ms_prompt;
	char		**path;
	// int			type;
	t_block		*head;
	t_minishell	ms_params;

	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	tgetent(0, getenv("TERM"));
	ft_memset(&ms_params, 0, sizeof(t_minishell));
	save_terminal_params(&ms_params);
	toggle_control_character(VQUIT, _POSIX_VDISABLE);
	t_prompt	prompt_params;

	// set_sig_handler();
	// if (!refresh_prompt_param(&prompt_params))
		// return (1);
	// res = NULL;
	// type = -1;
	ms_params.envp = get_env_var(env);
	(void)ac;
	(void)av;
	int fd = get_my_history();
	if (fd == -1)
		return (2);
	path = ft_split(getenv("PATH"), ':');
	while (1)
	{
		if (!refresh_prompt_param(&prompt_params))
			return (exit_ms(ms_params, 0, "prompt"), 0);
		ensure_prompt_position();
		ms_prompt = build_prompt(prompt_params);
		tmp = readline(ms_prompt);
		free(ms_prompt);
		if (!tmp)
			continue;
		my_add_history(rl_line_buffer, fd);
		head = new_block();
		if (!path)
			return (perror(""), 0);
		parse_cmds(&head, tmp, path);
		if (errno)
			return (exit_ms(ms_params, 2, "parsing"), 0);
		io_manager(head);
		if (errno)
			ms_perror("minishell", "io_manager", strerror(errno));
		ms_params.head = head;
		execute_cmds(head, &ms_params);
		wait_children(ms_params.children);
		free_children(&ms_params.children);
		flood_free(head);
	}
	ft_strsfree(path);
	free(rl_prompt);
	free(rl_line_buffer);
	//execute_t_block_cmd(head, &type, ms_params, head->io_tab);
		// execute_cmds(head, );
	// free(rl_prompt);
	// close(fd);
	// ft_putnbr_fd(isatty(ms_params.stdin_fileno), 36);
	return (exit_ms(ms_params, 0, NULL), 0);
}
