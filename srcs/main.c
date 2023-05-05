/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/05 07:33:36 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <ms_define.h>
#include <ms_signal.h>
#include <ms_parsing.h>
#include <ms_prompt.h>
#include <ms_env_function.h>
#include <libft.h>
#include <sys/stat.h>
#include <ms_history.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ms_exec.h>
#include <ncurses.h>
#include <term.h>

t_minishell	g_ms_params;

bool	is_line_empty(char *u_in)
{
	if (!*(u_in + pass_whitespaces(u_in)))
	{
		set_env_exit_var(0);
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
	exit_ms(extract_exit_code(g_ms_params.last_exit_code), "main_exit(-c)");
}

bool	is_valid_cmd_line(char *u_in)
{
	if (!u_in || is_line_empty(u_in))
	{
		set_env_exit_var(0);
		return (free(u_in), false);
	}
	ms_add_history(u_in);
	if (!parse_user_input(u_in))
		return (false);
	return (true);
}

bool	init_and_parse_input(char **av)
{

	if (g_ms_params.flags & C_FLAG)
		flag_case(av);
	else
	{
		g_ms_params.input_fd = init_prompt();
		if (g_ms_params.input_fd == -1)
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
	while (*user_input && quotes)
	{
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
		update_quotes(following_part, &quotes);
		free(following_part);
	}
	printf("==%s==\n", *user_input);
	if (errno)
		exit_ms(2, "ms_gnl");
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
		if (errno)
			exit_ms(2, "");
	}
	return (1);
}
