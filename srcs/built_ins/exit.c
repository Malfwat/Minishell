/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 13:37:17 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/26 23:08:44 by malfwa           ###   ########.fr       */
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
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
			return (false);
	}
	return (true);
}

void	ms_exit_builtin(t_minishell *ms_params, t_exec_vars vars, t_fd fd[2])
{
	int	length;
	int	exit_value;

	length = 0;
	if (fd[0] >= 0)
		close(fd[0]);
	if (fd[1] >= 0)
		close(fd[1]);
	ft_putstr_fd("exit\n", ms_params->stdin_fileno);
	while (vars.argv[length + 1])
		length++;
	if (!length)
		return (exit_ms(*ms_params, 0, NULL));
	else if (!ft_isnum(vars.argv[1]))
	{
		ms_perror("minishell: exit", vars.argv[1], "numeric argument required");
		return (exit_ms(*ms_params, 2, NULL));
	}
	else if (length != 1)
		return (ms_perror("minishell", "exit", "too many arguments"));
	exit_value = ft_atoi(vars.argv[1]);
	return (free_exec_vars(vars), exit_ms(*ms_params, exit_value, NULL));
}
