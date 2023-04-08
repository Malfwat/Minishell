/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_params_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 04:37:26 by marvin            #+#    #+#             */
/*   Updated: 2023/04/08 04:37:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <struct_ms.h>

bool	restore_terminal_params(struct termios saved_params)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &saved_params) == -1)
		return (false);
}

bool	save_terminal_params(t_minishell *ms_params)
{
	if (tcgetattr(STDIN_FILENO, &ms_params->saved_params) == -1)
		return (false);
}

bool	toggle_control_character(int control_character, int mode)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return (false);
	// term.c_lflag &= ~ECHOCTL; // est-ce nécessaire ?
	term.c_cc[control_character] = mode; // _POSIX_VDISABLE; // VQUIT, ...
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return (false);
	return (true);
}
