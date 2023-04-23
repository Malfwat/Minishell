/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 13:37:17 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/23 14:17:40 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <ms_define.h>
#include <struct_ms.h>

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

void	ms_exit_builtin(t_minishell ms_params, char **argv, t_fd fd)
{
	int	length;

	length = 0;
	if (fd == INIT_FD_VALUE)
		fd = 1;
	ft_putstr_fd("exit\n", fd);
	while (argv[length])
		length++;
	if (!length)
		return (exit_ms(ms_params, 0, NULL));
	else if (!ft_isnum(argv[0]))
	{
		ms_perror("minishell: exit", argv[0], "numeric argument required");
		return (exit_ms(ms_params, 2, NULL));
	}
	else if (length != 1)
	{
		ms_perror("minishell", "exit", "too many arguments");
		return ;
	}
	return (exit_ms(ms_params, ft_atoi(argv[0]), NULL));
}