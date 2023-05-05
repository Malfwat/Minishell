/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 03:16:49 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:21:51 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <termcap.h>
#include <ms_prompt.h>
#include <ms_env_function.h>
#include <ms_define.h>
#include <ms_struct.h>
#include <signal.h>
#include <ms_history.h>
#include <ncurses.h>

void	print_usage(void)
{
	ft_putstr_fd("Usage: ./minishell [-c arg]\n", 2);
	ft_putstr_fd("   -c arg: ", 2);
	ft_putstr_fd("if present then commands are read \
    from the first non-option argument \033[4marg\033[0m\n", 2);
}
