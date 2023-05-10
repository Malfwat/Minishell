/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:38:43 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/04 00:39:51 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <libft.h>
#include <minishell.h>

bool	pwd(t_fd fd)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (fd == INIT_FD_VALUE)
		fd = 1;
	if (!errno)
		ft_putendl_fd(current_dir, fd);
	else
	{
		free(current_dir);
		exit_ms(2, "pwd");
	}
	free(current_dir);
	return (true);
}
