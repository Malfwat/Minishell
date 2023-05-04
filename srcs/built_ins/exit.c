/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 13:37:17 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/04 02:26:03 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <ms_define.h>
#include <struct_ms.h>
#include <exec_ms.h>

bool	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (!*str)
		return (false);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
			return (false);
	}
	return (true);
}

#include <stdio.h>

bool	ms_exit_builtin(t_exec_vars vars, t_fd fd[2])
{
	int		length;
	int		exit_value;

	length = 0;
	my_close(fd[0], fd[1]);
	gnl_force_finish(1, g_ms_params.readline_pipe[0]);
	ft_putstr_fd("exit\n", g_ms_params.stdin_fileno);
	while (vars.argv[length + 1])
		length++;
	if (!length)
		return (free_exec_vars(vars), exit_ms(0, "exi1t"), true);
	else if (!ft_isnum(vars.argv[1]))
	{
		ms_perror("minishell: exit", vars.argv[1], "numeric argument required");
		return (free_exec_vars(vars), exit_ms(2, "ex2it"), true);
	}
	else if (length != 1)
		return (ms_perror("minishell", "exit", "too many arguments"), false);
	exit_value = ft_atoi(vars.argv[1]);
	return (free_exec_vars(vars), exit_ms(exit_value, "ex3it"), true);
}

