/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_usage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 03:16:49 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/07 13:43:50 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	print_usage(void)
{
	ft_putstr_fd("Usage: ./minishell [-c arg]\n", 2);
	ft_putstr_fd("   -c arg: ", 2);
	ft_putstr_fd("if present then commands are read \
    from the first non-option argument \033[4marg\033[0m\n", 2);
}
