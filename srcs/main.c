/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/03 07:43:59 by malfwa           ###   ########.fr       */
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
#include <exec_ms.h>
#include <ncurses.h>
#include <term.h>

t_minishell	g_ms_params;



// void	handler_func(int num)
// {
// 	char		*ms_prompt_up;
// 	int			last_exit_code;

// 	if (g_ms_params.children)
// 		return ;
// 	last_exit_code = 130;
// 	errno = 0;
// 	(void)num;
// 	if (!refresh_prompt_param(&g_ms_params.prompt_params, last_exit_code))
// 		exit_ms(g_ms_params, 0, "promp2t");
// 	if (g_ms_params.heredoc_pid)
// 		kill(g_ms_params.heredoc_pid, SIGTERM);
// 	write(1, "\n", 1);
// 	ms_prompt_up = build_prompt(&g_ms_params.prompt_params, P_HEADER);
// 	ft_putstr_fd(ms_prompt_up, 1);
// 	free(ms_prompt_up);
// 	free_prompt_params(&g_ms_params.prompt_params);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

bool	is_line_empty(char *u_in)
{
	if (!*(u_in + pass_whitespaces(u_in)))
	{
		free(find_env_var(g_ms_params.envp, "?")->var_value);
		find_env_var(g_ms_params.envp, "?")->var_value = ft_strdup("0");
		g_ms_params.last_exit_code = 0;
		return (true);
	}
	return (false);
}

void	flag_case(char **av)
{
	char	*u_in;

	av[2] += pass_whitespaces(av[2]);
	u_in = ft_strdup(av[2]);
	if (!u_in || !parse_user_input(u_in))
		exit_ms(extract_exit_code(g_ms_params.last_exit_code), \
		"parse (-c flag case)");
	execute_commands(g_ms_params.head);
	if (wait_children() == -1)
		exit_ms(2, "waitpid");
	free_children(&g_ms_params.children);
	flood_free(g_ms_params.head);
	g_ms_params.head = (t_block *){0};
	if (g_ms_params.flags & C_FLAG)
		exit_ms(extract_exit_code(g_ms_params.last_exit_code), "main_exit(-c)");
}

bool	is_valid_cmd_line(char *u_in)
{
	if (!u_in || is_line_empty(u_in))
		return (free(u_in), false);
	ms_add_history(u_in);
	if (!parse_user_input(u_in))
		return (false);
	return (true);
}

bool	init_and_parse_input(char **av)
{

	if (g_ms_params.flags & C_FLAG)
		return (flag_case(av), true);
	else
	{
		g_ms_params.input_fd = init_prompt();
		if (g_ms_params.input_fd < 3)
			return (false);	
	}
	return (true);
}

void	ms_gnl(t_fd fd, char **user_input, bool conserve_nl)
{
	char	*following_part;
	int		len;
	char	c[2];
	char	*tmp;
	char	*quotes;

	ft_bzero(c, 2);
	get_next_line(fd, user_input);
	len = 0;
	if (*user_input)
		len = ft_strlen(*user_input);
	quotes = NULL;
	update_quotes(*user_input, &quotes);
	while (*user_input && ((len >= 2 && (*user_input)[len - 1] == '\n' \
		&& (*user_input)[len - 2] == '\\') || quotes))
	{
		if (len > 2 && (*user_input)[len - 2] == '\\')
		{
			(*user_input)[len - 1] = 0;
			(*user_input)[len - 2] = 0;
		}
		len = ft_strlen(*user_input);
		if (quotes)
			c[0] = *quotes;
		get_next_line(fd, &following_part);
		if (!following_part)
			break ;
		tmp = *user_input;
		*user_input = ft_strjoin(*user_input, following_part);
		free(tmp);
		if (*user_input)
			len = ft_strlen(*user_input);
		quotes = c;
		update_quotes(*user_input, &quotes);
		free(following_part);
	}
	if (*user_input)
		len = ft_strlen(*user_input);
	if (!conserve_nl && *user_input && (*user_input)[len - 1] == '\n')
		(*user_input)[len - 1] = 0;
}

int	main(int ac, char **av, char **envp)
{
	char	*user_input;

	if (!init_minishell(&g_ms_params, ac, av, envp))
		return (1);
	user_input = NULL;
	while (1)
	{
		if (!init_and_parse_input(av))
			continue ;
		ms_gnl(g_ms_params.input_fd, &user_input, false);
		while (1)
		{
			if (is_valid_cmd_line(user_input))
			{
				execute_commands(g_ms_params.head);
				if (wait_children() == -1)
					exit_ms(2, "waitpid");
				free_children(&g_ms_params.children);
			}
			flood_free(g_ms_params.head);
			g_ms_params.head = (t_block *){0};
			ms_gnl(g_ms_params.input_fd, &user_input, false);
			if (!user_input)
				break ;
		}
		close(g_ms_params.input_fd);
	}
	return (1);
}
