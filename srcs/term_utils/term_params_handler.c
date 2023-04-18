/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_params_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 04:37:26 by marvin            #+#    #+#             */
/*   Updated: 2023/04/18 21:09:51 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <struct_ms.h>
#include <stdio.h>

bool	restore_terminal_params(struct termios saved_params, t_fd stdin_fileno)
{
	(void)stdin_fileno;
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
