/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 11:20:41 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/11 21:06:47 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdarg.h>
#include <libft.h>
#include <unistd.h>

void	ms_echo(bool nl, int nb, ...)
{
	va_list	ap;

	va_start(ap, nb);
	while (nb--)
	{
		ft_putstr_fd(va_arg(ap, char *), 1);
		if (nb)
			write(1, " ", 1);
	}
	if (nl)
		write(1, "\n", 1);
}
