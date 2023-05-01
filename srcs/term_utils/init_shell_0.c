/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:40:38 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/01 20:39:45 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <termcap.h>
#include <prompt.h>
#include <env_function.h>
#include <ms_define.h>
#include <struct_ms.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <history.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ncurses.h>
#include <unistd.h>

bool	init_termois_struct(void)
{
	struct termios	term;
	char			*sc_cursor_pos;

	sc_cursor_pos = tgetstr("u7", NULL);
	if (!sc_cursor_pos)
		return (false);
	tcgetattr(0, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &term);
	write(0, sc_cursor_pos, ft_strlen(sc_cursor_pos));
	return (true);
}

int	get_cursor_position(void)
{
	struct termios	restore;
	char			buf[100];
	int				i;
	int				ret;
	char			*digit_index[1];

	tcgetattr(0, &restore);
	if (!init_termois_struct())
		return (tcsetattr(0, TCSANOW, &restore), -1);
	ft_bzero(buf, 10);
	ret = 1;
	i = -1;
	while (ret > 0 && ++i < 9)
	{
		ret = read(STDIN_FILENO, &buf[i], 1);
		if (ret <= 0 || buf[i] == 'R')
			break ;
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

void	handler_readline(int num)
{
	(void)num;
	my_close(g_ms_params.readline_pipe[1], -2);
	errno = 0;
	write(g_ms_params.stdin_fileno, "\n", 1);
	free(g_ms_params.ms_prompt);
	g_ms_params.ms_prompt = NULL;
	exit_ms(-2, "handler readline");
}

void	do_nothing(int num)
{
	(void)num;
}

t_fd	init_prompt(void)
{

	int					status;
	int					exit_value;
	char				*tmp;
	// char		*ms_prompt_up;
	int			last_exit_code;

	last_exit_code = g_ms_params.last_exit_code;
	if (!refresh_prompt_param(&g_ms_params.prompt_params, last_exit_code))
		exit_ms(0, "prompt1");
	ensure_prompt_position();
	// ft_putstr_fd(ms_prompt_up, 1);
	// free(ms_prompt_up);
	// g_ms_params.ms_prompt = build_prompt(&g_ms_params.prompt_params, P_FOOTER);
	g_ms_params.ms_prompt = build_prompt(&g_ms_params.prompt_params, P_HEADER);
	free_prompt_params(&g_ms_params.prompt_params);
	if (errno || pipe(g_ms_params.readline_pipe))
		exit_ms(0, "prompt2");
	g_ms_params.readline_pid = fork();
	if (!g_ms_params.readline_pid)
	{
		signal(SIGINT, handler_readline);
		my_close(g_ms_params.readline_pipe[0], -2);
		tmp = readline(g_ms_params.ms_prompt);
		while (1)
		{
			if (tmp)
				write(g_ms_params.readline_pipe[1], tmp, ft_strlen(tmp));
			else
			{
				write(g_ms_params.stdin_fileno, "exit\n", 5);
				my_close(g_ms_params.readline_pipe[1], -2);
				exit_ms(1, "fork_readline");	
			}
			if (tmp[ft_strlen(tmp) - 1] == '\\')
			{
				write(g_ms_params.readline_pipe[1], "\b", 1);
				free(tmp);
				tmp = readline("> ");
			}
			else
			{
				free(tmp);
				break ;
			}
		}
		my_close(g_ms_params.readline_pipe[1], -2);
		free(g_ms_params.ms_prompt);
		exit_ms(0, "fork_readline");
	}
	waitpid(g_ms_params.readline_pid, &status, 0);
	exit_value = extract_exit_code(status);
	if (exit_value == 1)
	{
		free(g_ms_params.ms_prompt);
		my_close(g_ms_params.readline_pipe[1], g_ms_params.readline_pipe[0]);
		exit_ms(0, "init prompt");
	}
	if (exit_value != 0 || errno)
		return (g_ms_params.last_exit_code = 130, my_close(g_ms_params.readline_pipe[1], g_ms_params.readline_pipe[0]), -1);
	errno = 0;
	my_close(g_ms_params.readline_pipe[1], -2);
	free(g_ms_params.ms_prompt);
	g_ms_params.ms_prompt = NULL;
	g_ms_params.last_exit_code = 0;
	return (g_ms_params.readline_pipe[0]);
}

bool	init_minishell(t_minishell *ms_params, int ac, char **av, char **envp)
{
	(void)ms_params;
	ft_memset(&g_ms_params, 0, sizeof(t_minishell));
	if (ac == 3 && !ft_strcmp(av[1], "-c"))
		g_ms_params.flags |= C_FLAG;
	if (ac > 1 && (g_ms_params.flags & C_FLAG) == 0)
		return (print_usage(), false);
	if ((g_ms_params.flags & C_FLAG) == 0
		&& (!isatty(0) || !isatty(1) || !isatty(2)))
		return (perror("minishell"), false);
	if ((g_ms_params.flags & C_FLAG) == 0)
	{
		tgetent(0, getenv("TERM"));
		save_terminal_params(&g_ms_params);
		toggle_control_character(VQUIT, _POSIX_VDISABLE);
		signal(SIGINT, &do_nothing);
		g_ms_params.history_fd = get_my_history();
		if (g_ms_params.history_fd == -1)
			return (false);
	}
	g_ms_params.envp = get_env(envp);
	g_ms_params.previous_directory = getcwd(NULL, 0);
	if (errno)
		return (perror("minishell"), false);
	return (true);
}
