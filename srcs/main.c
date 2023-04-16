/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/15 23:54:47 by hateisse         ###   ########.fr       */
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

t_minishell	ms_params_global;

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
	}
	if (block->io_tab[1] != INIT_FD_VALUE)
	{
		int d, c;

		d = dup2(block->io_tab[1], 1);
		c = close(block->io_tab[1]);
		if (d == -1 || c == -1)
		{
			return (false);
		}
	}
	if (block->pipe_next)
	{
		close(block->pipe_next->io_tab[0]);
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
	if (!init_exec_io(block, ms_params))
		return ;
	exec_vars = init_exec_vars(*ms_params, block);
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
	if (block->operator == AND_OPERATOR|| block->operator == OR_OPERATOR
		|| block->operator == SEMI_COLON)
	{
		if (waitpid(block->cmd.pid, &block->cmd.exit_value, 0) == -1)
			exit_ms(*ms_params, 2, "waitpid");
		ms_params->last_exit_code = block->cmd.exit_value;
	}
	else
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

int	wait_children(t_minishell *ms_params)
{
	int		status;
	t_pids	*children;

	children = ms_params->children;
	while (children)
	{
		if (waitpid(children->pid, &status, 0) == -1)
			return (-1);
		children = children->next;
	}
	free_children(&ms_params->children);
	ms_params->last_exit_code = status;
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
		my_dup(block);
		execute_commands(block->sub, ms_params); // on passe au contenu du subshell immediatement
		exit_ms(*ms_params, extract_exit_code(wait_children(ms_params)), "subshell");
	}
	close_sub_fds(block->sub);
	if (block->io_tab[0] != INIT_FD_VALUE)
	{
		if (close(block->io_tab[0]) == -1)
		{
			if (block->io_tab[1] != INIT_FD_VALUE)
			{
				close(block->io_tab[1]);
			}
			exit_ms(*ms_params, 2, "subshell");
		}
	}
	if (block->io_tab[1] != INIT_FD_VALUE)
	{
		if (close(block->io_tab[1]) == -1)
			exit_ms(*ms_params, 2, "subshell");
	}
	return (sub_pid);
}

int	execute_commands(t_block *block, t_minishell *ms_params)
{
	t_block	*next_block_to_execute;
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
		block->cmd.pid = create_subshell(block, ms_params);
		if (block->operator == AND_OPERATOR|| block->operator == OR_OPERATOR
			|| block->operator == SEMI_COLON)
		{
			if (waitpid(block->cmd.pid, &block->cmd.exit_value, 0) == -1)
				exit_ms(*ms_params, 2, "waitpid");
			ms_params->last_exit_code = block->cmd.exit_value;
		}
		else
			store_pid(block->cmd.pid, &ms_params->children);
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
		execute_commands(next_block_to_execute, ms_params);
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

bool	init_minishell(t_minishell *ms_params, char **envp)
{
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), false);
	tgetent(0, getenv("TERM"));
	ft_memset(ms_params, 0, sizeof(t_minishell));
	save_terminal_params(ms_params);
	toggle_control_character(VQUIT, _POSIX_VDISABLE);
	// set_sig_handler();
	// if (!refresh_prompt_param(&ms_prompt.prompt_params))
		// return (1);
	// res = NULL;
	// type = -1;
	ms_params->history_fd = get_my_history();
	if (ms_params->history_fd == -1)
		return (false);
	ms_params->envp = get_env_var(envp);
	if (errno)
		return (perror("minishell"), false);
	return (true);
}

void	init_prompt(t_minishell *ms_params, char **user_input)
{
	char		*ms_prompt;
	int			last_exit_code;

	last_exit_code = ms_params->last_exit_code;
	if (!refresh_prompt_param(&ms_params->prompt_params, last_exit_code))
		exit_ms(*ms_params, 0, "prompt");
	ensure_prompt_position();
	ms_prompt = build_prompt(&ms_params->prompt_params, false);
	if (!ms_prompt || errno)
		exit_ms(*ms_params, 0, "prompt");
	*user_input = readline(ms_prompt);
	errno = 0;
	free(ms_prompt);
	ms_params->last_exit_code = 0;
}

bool	parse_user_input(t_minishell *ms_params, char *user_input)
{
	t_block		*head;

	head = new_block();
	if (parse_cmds(&head, user_input) == false)
		return (false);
	if (errno)
		exit_ms(*ms_params, 2, "parsing");
	hd_manager(head);
	if (errno)
		exit_ms(*ms_params, 2, "io_manager");
	ms_params->head = head;
	return (true);
}

// void clear_line() {
// 	printf("\33[2K\r");
// }

// void redisplay_full_prompt(vp) {
// 	clear_line();
// 	printf("%s%s\n", rl_prompt, rl_line_buffer);
// }

void	handler_func(int num)
{
	(void)num;
	char	*prompt_header;
	
	prompt_header = build_prompt(&ms_params->prompt_params, true);
	write(1, "\n", 1);
	ft_putstr_fd(prompt_header, 2);
	free(prompt_header);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	// printf("\n");
	// rl_replace_line("", 0); // Clear the previous text
	// rl_forced_update_display();
}

int	main(int ac, char **av, char **envp)
{
	char		*user_input;

	if (!init_minishell(&ms_params_global, envp))
		return (1);
	(void)ac;
	(void)av;
	signal(SIGINT, &handler_func);
	while (1)
	{
		init_prompt(&ms_params_global, &user_input);
		if (!user_input)
			continue;

		ms_add_history(rl_line_buffer, ms_params_global.history_fd);

		if (!parse_user_input(&ms_params_global, user_input))
			continue ;

		execute_commands(ms_params_global.head, &ms_params_global);
		if (wait_children(&ms_params_global) == -1)
			exit_ms(ms_params_global, 2, "waitpid");
		free_children(&ms_params_global.children);
		flood_free(ms_params_global.head);
	}
	return (0);
}
