/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 11:20:41 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 19:12:01 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdarg.h>
#include <libft.h>
#include <ms_define.h>
#include <string.h>
#include <minishell.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>

bool	check_nl_arg(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-')
		return (false);
	while (arg[i])
	{
		if (arg[i++] != 'n')
			return (false);
	}
	return (true);
}

void	ms_echo(char **tab, t_fd fd)
{
	int		i;
	bool	nl;

	i = 0;
	nl = true;
	if (fd == INIT_FD_VALUE)
		fd = 1;
	if (!tab)
		return ((void)write(fd, "\n\n", 2));
	while (tab[i] && check_nl_arg(tab[i]))
		i++;
	if (i != 0)
		nl = false;
	while (tab[i])
	{
		ft_putstr_fd(tab[i++], fd);
		if (tab[i])
			write(fd, " ", 1);
	}
	if (nl)
		write(fd, "\n", 1);
	if (errno)
		ms_perror("minishell: echo", "write error", strerror(errno));
	errno = 0;
}
