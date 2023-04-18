/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:40:38 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/18 21:35:04 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <termcap.h>
#include <prompt.h>
#include <env_function.h>
#include <ms_define.h>
#include <struct_ms.h>
#include <signal.h>
#include <history.h>

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

void	init_prompt(t_minishell *ms_params, char **user_input)
{
	char		*ms_prompt_up;
	char		*ms_prompt;
	int			last_exit_code;

	last_exit_code = ms_params->last_exit_code;
	if (!refresh_prompt_param(&ms_params->prompt_params, last_exit_code))
		exit_ms(*ms_params, 0, "prompt1");
	ensure_prompt_position();
	ms_prompt_up = build_prompt(&ms_params->prompt_params, UP);
	ft_putstr_fd(ms_prompt_up, 1);
	free(ms_prompt_up);
	ms_prompt = build_prompt(&ms_params->prompt_params, DOWN);
	free_prompt_params(&ms_params->prompt_params);
	if (errno)
		exit_ms(*ms_params, 0, "prompt2");
	*user_input = readline(ms_prompt);
	errno = 0;
	free(ms_prompt);
	ms_params->last_exit_code = 0;
}

bool	init_minishell(t_minishell *ms_params, char **envp)
{
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), false);
	tgetent(0, getenv("TERM"));
	ft_memset(ms_params, 0, sizeof(t_minishell));
	save_terminal_params(ms_params);
	toggle_control_character(VQUIT, _POSIX_VDISABLE);
	signal(SIGINT, &handler_func);
	ms_params->history_fd = get_my_history(ms_params);
	if (ms_params->history_fd == -1)
		return (false);
	ms_params->envp = get_env_var(envp);
	if (errno)
		return (perror("minishell"), false);
	return (true);
}
