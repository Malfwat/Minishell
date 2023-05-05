/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_user_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 00:54:04 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/05 06:16:13 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_struct.h>
#include <ms_define.h>
#include <minishell.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <ms_parsing.h>

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
