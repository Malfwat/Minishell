/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_user_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 00:54:04 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 23:23:20 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_struct.h>
#include <ms_define.h>
#include <minishell.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <ms_parsing.h>
#include <ms_history.h>
#include <ms_exec.h>
#include <ms_heredoc.h>

bool	parse_user_input(char *user_input)
{
	t_block		*head;

	head = new_block();
	if (!*user_input)
		return (true);
	if (parse_cmds(&head, user_input) == false)
	{
		g_ms_params.last_exit_code = 2 << 8;
		return (flood_free(head), false);
	}
	if (errno)
		exit_ms(2, "parsing");
	g_ms_params.head = head;
	hd_manager(head);
	if (g_ms_params.heredoc_pid == -3)
	{
		g_ms_params.heredoc_pid = 0;
		g_ms_params.last_exit_code = 130;
		return (false);
	}
	g_ms_params.heredoc_pid = 0;
	if (errno)
		exit_ms(2, "io_manager");
	return (true);
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
