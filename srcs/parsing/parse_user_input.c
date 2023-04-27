/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_user_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 00:54:04 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/28 01:07:51 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <ms_define.h>
#include <minishell.h>
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
	hd_manager(head);
	if (errno)
		exit_ms(*ms_params, 2, "io_manager");
	ms_params->head = head;
	return (true);
}
