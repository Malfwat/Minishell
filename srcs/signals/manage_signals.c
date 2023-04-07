/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:26:27 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/07 16:50:03 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

void	sigquit_handler(int num)
{
	(void)num;
	// rl_replace_line("\b\b", 0);
	// rl_on_new_line();
	// rl_redisplay();
	return ;
}

bool	set_sig_handler(void)
{
	signal(SIGQUIT, sigquit_handler);
	return (true);
}

bool	disable_cc_backslash(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return (false);
	term.c_lflag &= ~ECHOCTL;
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return (false);
	return (true);
}
