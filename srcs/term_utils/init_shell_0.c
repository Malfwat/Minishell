/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:40:38 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:23:04 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <termcap.h>
#include <ms_prompt.h>
#include <ms_env_function.h>
#include <ms_define.h>
#include <ms_struct.h>
#include <sys/types.h>
#include <signal.h>
#include <ms_signal.h>
#include <stdlib.h>
#include <ms_history.h>
#include <fcntl.h>
#include <ms_exec.h>
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

char	*check_for_quotes(char *str, char *quote)
{
	char	*closing_quote;
	char	*quote_found;

	if (!str || !*str)
		return (quote);
	quote_found = ft_strchr_set(str, "'\"");
	if (quote && quote_found && *quote_found == *quote)
		return (check_for_quotes(quote + 1, NULL));
	else if (quote_found && !quote)
	{
		closing_quote = ft_strchr(quote_found + 1, *quote_found);
		if (closing_quote)
			return (check_for_quotes(closing_quote + 1, NULL));
		return (quote_found);
	}
	return (quote);
}

void	update_quotes(char *str, char **quotes)
{
	*quotes = check_for_quotes(str, *quotes);
}

void	ms_readline(char *tmp, char *quote)
{
	char	c[2];
	bool	erase_backslash;
	
	ft_bzero(c, 2);
	erase_backslash = 0;
	if (tmp)
	{
		if ((ft_strlen(tmp) > 1 && tmp[ft_strlen(tmp) - 1] == '\\') && !quote)
			erase_backslash = 1;
		write(g_ms_params.readline_pipe[1], tmp, ft_strlen(tmp) - erase_backslash);
		if (quote)
			write(g_ms_params.readline_pipe[1], "\n", 1);
	}
	else
	{
		write(g_ms_params.stdin_fileno, "exit\n", 5);
		my_close(g_ms_params.readline_pipe[1], -2);
		return (errno = 0, exit_ms(1, "ms_readline"));
	}
	if (!quote && tmp[ft_strlen(tmp) - 1] != '\\')
		return (free(tmp));
	if (quote)
		c[0] = *quote;
	free(tmp);
	tmp = readline("> ");
	if (errno == EINTR)
		errno = 0;
	if (!tmp)
	{
		ms_perror("minishell", "unexpected EOF while looking for matching", c);
		ms_perror("minishell", "syntax error", "unexpected end of file");
		my_close(g_ms_params.readline_pipe[1], -2);
		return (errno = 0, exit_ms(2, "ms_readline"));
	}
	quote = (char *)&c[0];
	update_quotes(tmp, &quote);
	return (ms_readline(tmp, quote));
}

void	readline_child(void)
{
	char				*tmp;
	char				*quotes;
	
	my_close(g_ms_params.readline_pipe[0], -2);
	if (!refresh_prompt_param(&g_ms_params.prompt_params, \
		g_ms_params.last_exit_code))
	{
		my_close(g_ms_params.readline_pipe[1], -2);
		exit_ms(0, "prompt1");
	}
	ensure_prompt_position();
	g_ms_params.ms_prompt = build_prompt(&g_ms_params.prompt_params, P_HEADER);
	free_prompt_params(&g_ms_params.prompt_params);
	quotes = NULL;
	signal(SIGINT, handler_readline);
	tmp = readline(g_ms_params.ms_prompt);
	if (errno == EINTR)
		errno = 0;
	if (tmp)
	{
		quotes = check_for_quotes(tmp, quotes);
		if (quotes && quotes[1])
			update_quotes(quotes + 1, &quotes);
	}
	free(g_ms_params.ms_prompt);
	g_ms_params.ms_prompt = NULL;
	ms_readline(tmp, quotes);
	my_close(g_ms_params.readline_pipe[1], -2);
	exit_ms(0, "fork_readline");
}

t_fd	init_prompt(void)
{
	int					status;
	int					exit_value;

	if (pipe(g_ms_params.readline_pipe))
		exit_ms(0, "prompt pipe");
	g_ms_params.readline_pid = fork();
	if (!g_ms_params.readline_pid)
		readline_child();
	waitpid(g_ms_params.readline_pid, &status, 0);
	exit_value = extract_exit_code(status);
	if (exit_value || errno)
	{
		g_ms_params.last_exit_code = status;
		my_close(g_ms_params.readline_pipe[1], g_ms_params.readline_pipe[0]);
		if (exit_value == 1)
			exit_ms(0, "init prompt");
		return (-1);
	}
	my_close(g_ms_params.readline_pipe[1], -2);
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
		if (tgetent(0, getenv("TERM")) == -1)
			return (ms_perror("minishell", "tgetent", "couldn't load termcaps"), false);
		save_terminal_params(&g_ms_params);
		toggle_control_character(VQUIT, _POSIX_VDISABLE);
		signal(SIGINT, &do_nothing);
		g_ms_params.history_fd = get_my_history();
	}
	g_ms_params.envp = get_env(envp);
	g_ms_params.previous_directory = getcwd(NULL, 0);
	if (errno)
		return (perror("minishell"), false);
	return (true);
}
