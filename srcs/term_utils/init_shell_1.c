/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 03:16:49 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 03:18:23 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <termcap.h>
#include <prompt.h>
#include <env_function.h>
#include <ms_define.h>
#include <struct_ms.h>
#include <signal.h>
#include <history.h>
#include <ncurses.h>

int	ft1(int a, int b)
{
	rl_replace_line("echo test", 0);
	return ((void)a, (void)b, 1);
}

void	init_keyhooks(void)
{
	// rl_bind_key(27, NULL);
	rl_bind_keyseq("\\e[A", ft1);
}

void	print_usage(void)
{
	ft_putstr_fd("Usage: ./minishell [-c arg]\n", 2);
	ft_putstr_fd("   -c arg: ", 2);
	ft_putstr_fd("if present then commands are read \
    from the first non-option argument \033[4marg\033[0m\n", 2);
}
