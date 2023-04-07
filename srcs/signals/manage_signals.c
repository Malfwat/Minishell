/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:01:41 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/07 04:49:17 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <readline/readline.h>

void	sigquit_handler(int num)
{
	(void)num;
	// rl_replace_line(rl_prompt, 0);
	rl_on_new_line();
	rl_redisplay();
	return ;
}

void	set_sig_handler(void)
{
	signal(SIGQUIT, sigquit_handler);
}