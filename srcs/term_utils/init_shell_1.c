/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 03:16:49 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/28 01:13:06 by hateisse         ###   ########.fr       */
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

void	print_usage(void)
{
	ft_putstr_fd("Usage: ./minishell [-c arg]\n", 2);
	ft_putstr_fd("   -c arg: ", 2);
	ft_putstr_fd("if present then commands are read \
    from the first non-option argument \033[4marg\033[0m\n", 2);
}
