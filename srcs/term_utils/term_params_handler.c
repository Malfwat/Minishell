/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_params_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 04:37:26 by marvin            #+#    #+#             */
/*   Updated: 2023/05/11 17:40:24 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <ms_struct.h>
#include <stdio.h>
#include <minishell.h>

bool	restore_terminal_params(void)
{
	struct termios	saved_params;
	t_fd			stdin_fileno;

	stdin_fileno = g_ms_params.stdin_fileno;
	saved_params = g_ms_params.saved_params;
	if (tcsetattr(stdin_fileno, TCSANOW, &saved_params) == -1)
		return (false);
	return (true);
}

bool	save_terminal_params(t_minishell *ms_params)
{
	ms_params->stdin_fileno = dup(0);
	if (tcgetattr(STDIN_FILENO, &ms_params->saved_params) == -1)
		return (false);
	return (true);
}

bool	toggle_control_character(int control_character, int mode)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return (false);
	term.c_cc[control_character] = mode;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return (false);
	return (true);
}
