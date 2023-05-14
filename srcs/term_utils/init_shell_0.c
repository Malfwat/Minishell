/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:40:38 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/14 11:53:01 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <libft.h>
#include <minishell.h>
#include <ms_define.h>
#include <ms_env_function.h>
#include <ms_exec.h>
#include <ms_history.h>
#include <ms_prompt.h>
#include <ms_readline.h>
#include <ms_signal.h>
#include <ms_struct.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <termcap.h>
#include <unistd.h>

bool	init_termios_struct(void)
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

int	error_init_prompt(int status, int exit_value)
{
	g_ms_params.last_exit_code = status;
	my_close(g_ms_params.readline_pipe[1], g_ms_params.readline_pipe[0]);
	if (exit_value == 1)
		exit_ms(0, "init prompt");
	return (-1);
}

t_fd	init_prompt(void)
{
	int	status;
	int	exit_value;

	status = 0;
	if (pipe(g_ms_params.readline_pipe))
		exit_ms(0, "prompt pipe");
	g_ms_params.readline_pid = fork();
	if (g_ms_params.readline_pid == -1)
	{
		my_close(g_ms_params.readline_pipe[1], g_ms_params.readline_pipe[0]);
		exit_ms(0, "prompt fork");
	}
	else if (!g_ms_params.readline_pid)
		readline_child();
	waitpid(g_ms_params.readline_pid, &status, 0);
	exit_value = extract_exit_code(status);
	if (exit_value || errno)
		return (error_init_prompt(status, exit_value));
	my_close(g_ms_params.readline_pipe[1], -2);
	g_ms_params.ms_prompt = NULL;
	return (g_ms_params.last_exit_code = 0, g_ms_params.readline_pipe[0]);
}

bool	init_minishell(t_minishell *ms_params, int ac, char **av, char **envp)
{
	(void)ms_params;
	ft_memset(&g_ms_params, 0, sizeof(t_minishell));
	if (ac == 3 && !ft_strcmp(av[1], "-c"))
		g_ms_params.flags |= C_FLAG;
	if (ac > 1 && (g_ms_params.flags & C_FLAG) == 0)
		return (print_usage(), false);
	if ((g_ms_params.flags & C_FLAG) == 0 && (!isatty(0) || !isatty(1)
			|| !isatty(2)))
		return (perror("minishell"), false);
	if ((g_ms_params.flags & C_FLAG) == 0)
	{
		if (tgetent(0, getenv("TERM")) == -1)
			return (ms_perror("minishell", "tgetent", \
			"couldn't load termcaps"), false);
		save_terminal_params(&g_ms_params);
		toggle_control_character(VQUIT, _POSIX_VDISABLE);
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		g_ms_params.history_fd = get_my_history();
	}
	g_ms_params.envp = get_env(envp);
	g_ms_params.previous_directory = getcwd(NULL, 0);
	if (errno)
		return (perror("minishell"), false);
	return (true);
}
