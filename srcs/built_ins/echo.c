/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 11:20:41 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 17:37:45 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdarg.h>
#include <libft.h>
#include <ms_define.h>
#include <unistd.h>

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
	{
		write(fd, "\n\n", 2);
		return ;
	}
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
}
