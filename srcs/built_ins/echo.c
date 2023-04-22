/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 11:20:41 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 05:15:53 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdarg.h>
#include <libft.h>
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

void	ms_echo(char **tab)
{
	int		i;
	bool	nl;
	
	i = 0;
	nl = true;
	if (!tab)
	{
		write(1, "\n\n", 2);
		return ;
	}
	while (tab[i] && check_nl_arg(tab[i]))
		i++;
	if (i != 0)
		nl = false;
	while (tab[i])
	{
		ft_putstr_fd(tab[i++], 1);
		if (tab[i])
			write(1, " ", 1);
	}
	if (nl)
		write(1, "\n", 1);
}
