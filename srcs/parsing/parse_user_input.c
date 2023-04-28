/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_user_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 00:54:04 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/28 04:48:58 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <ms_define.h>
#include <minishell.h>
#include <sys/wait.h>
#include <signal.h>
#include <parsing_ms.h>

bool	parse_user_input(t_minishell *ms_params, char *user_input)
{
	t_block		*head;

	head = new_block();
	if (parse_cmds(&head, user_input) == false)
	{
		ms_params->last_exit_code = SET_EXIT_CODE(2);
		return (flood_free(head), false);
	}
	if (errno)
		exit_ms(*ms_params, 2, "parsing");
	hd_manager(head, ms_params);
	if (ms_params->heredoc_pid == -3)
		return (ms_params->heredoc_pid = 0, flood_free(head), false);
	ms_params->heredoc_pid = 0;
	if (errno)
		exit_ms(*ms_params, 2, "io_manager");
	ms_params->head = head;
	return (true);
}
