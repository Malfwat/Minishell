/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/04 01:22:02 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <libft.h>
#include <stdbool.h>
#include <stdlib.h>
#include <minishell.h>
#include <unistd.h>
#include <parsing.h>

int	main(int ac, char **av)
{
	char	*res = NULL;
	int		i = 0;
	
	if (ac != 2)
		return (0);
	check_io_param(av[1], &i, &res);
	printf("[%d] res: '%s'\n", i, res);
}
