/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/14 21:07:14 by hateisse         ###   ########.fr       */
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


void handle_execve_failure(t_minishell ms_params, char *program_name);



void	close_block_fds(t_block *block)
{
	if (block->io_tab[0] > 2)
	{
		close(block->io_tab[0]);
		block->io_tab[0] = -2; // valeur quand non initialise
	}
	if (block->io_tab[1] > 2)
	{
		close(block->io_tab[1]);
		block->io_tab[1] = -2;
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

char	**build_path(t_minishell ms_params)
{
	char	**path;
	
	path = ft_split(find_env_var(ms_params.envp, "PATH")->var_value, ':');
	if (errno)
		exit_ms(ms_params, 2, "exec_build");
	return (path);
}

void	free_exec_vars(t_exec_vars exec_vars)
{
	free(exec_vars.argv);
	ft_strsfree(exec_vars.envp);
	ft_strsfree(exec_vars.path);
}


void	insert_t_args(t_args **head, t_args *current, t_args *new_lst)
{
	if (*head == current)
	{
		*head = new_lst;
	}
	else
	{
		current->prev->next = new_lst;
		new_lst->prev = current->prev;
	}
	if (current->next)
	{
		current->next->prev = last_args(new_lst);
	}
	last_args(new_lst)->next = current->next;
}

bool	rebuild_args(t_args **head, t_env_var *envp)
{
	t_args	*ptr;
	// t_args	*new_lst;
	// char	*joined_split_args;

	ptr = *head;
	while (ptr)
	{
		ptr->final_arg = join_splitted_arg(ptr->s_args, envp, true);
		if (errno)
			return (false);
		// new_lst = produce_t_args_from_str(joined_split_args);
		// if (errno)
			// return (free(joined_split_args), false);
		// free(joined_split_args);
		// insert_t_args(head, ptr, new_lst);
		ptr = ptr->next;
	}
	return (true);
}

t_exec_vars init_exec_vars(t_minishell ms_params, t_block *block)
{
	t_exec_vars exec_vars;
	char 		*tmp;
	
	if (!rebuild_args(&block->cmd.args, ms_params.envp))
		exit_ms(ms_params, 2, "exec_build");
	exec_vars.path = build_path(ms_params);
	get_cmd_path(exec_vars.path, &block->cmd.args->final_arg, &tmp);
	block->cmd.args->final_arg = tmp;
	exec_vars.argv = build_argv(&block->cmd.args);
	exec_vars.envp = build_envp(ms_params.envp);
	if (errno)
	{
		free_exec_vars(exec_vars);
		exit_ms(ms_params, 2, "exec_build");
	}
	return (exec_vars);
}

void	child_worker(t_block *block, t_minishell *ms_params, t_exec_vars exec_vars)
{
	if (!my_dup(block))
		return (free_exec_vars(exec_vars), exit_ms(*ms_params, 2, "exec dup"));
	execve(exec_vars.argv[0], exec_vars.argv, exec_vars.envp);
	free_exec_vars(exec_vars);
	if (block->io_tab[0] >= 0)
		close(block->io_tab[0]);
	if (block->io_tab[1] >= 0)
		close(block->io_tab[1]);
	if (block->pipe_next)
		close(block->pipe_next->io_tab[0]);
	handle_execve_failure(*ms_params, block->cmd.args->final_arg);
}

void	execute_t_block_cmd(t_block *block, t_minishell *ms_params)
{
	t_exec_vars	exec_vars;

	errno = 0;
	exec_vars = init_exec_vars(*ms_params, block);
	init_exec_io(block, ms_params->envp);
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
		if (waitpid(block->cmd.pid, &block->cmd.exit_value, 0) == -1)
			exit_ms(*ms_params, 2, "waitpid");
	store_pid(block->cmd.pid, &ms_params->children);
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
		if (waitpid(children->pid, &status, 0) == -1)
			return (-1);
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
	// int			type;
	t_block		*head;
	t_minishell	ms_params;

	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	tgetent(0, getenv("TERM"));
	ft_memset(&ms_params, 0, sizeof(t_minishell));
	save_terminal_params(&ms_params);
	toggle_control_character(VQUIT, _POSIX_VDISABLE);

	// set_sig_handler();
	// if (!refresh_prompt_param(&ms_prompt.prompt_params))
		// return (1);
	// res = NULL;
	// type = -1;
	
	ms_params.envp = get_env_var(env);
	(void)ac;
	(void)av;
	ms_params.history_fd = get_my_history();
	if (ms_params.history_fd == -1)
		return (2);
	while (1)
	{
		if (!refresh_prompt_param(&ms_params.prompt_params))
			return (exit_ms(ms_params, 0, "prompt"), 0);
		ensure_prompt_position();
		ms_prompt = build_prompt(&ms_params.prompt_params);
		tmp = readline(ms_prompt);
		free(ms_prompt);
		if (!tmp)
			continue;
		my_add_history(rl_line_buffer, ms_params.history_fd);
		head = new_block();
		if (parse_cmds(&head, tmp) == false)
			continue ;
		if (errno)
			return (exit_ms(ms_params, 2, "parsing"), 0);
		hd_manager(head);
		if (errno)
			ms_perror("minishell", "io_manager", strerror(errno));
		ms_params.head = head;
		execute_cmds(head, &ms_params);
		if (wait_children(ms_params.children) == -1)
			exit_ms(ms_params, 2, "waitpid");
		free_children(&ms_params.children);
		flood_free(head);
	}
	return (0);
}
