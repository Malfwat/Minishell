/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:38:43 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/11 19:51:08 by hateisse         ###   ########.fr       */
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
		ms_perror("minishell", "pwd", strerror(errno));
		errno = 0;
		free(current_dir);
		return (false);
	}
	errno = 0;
	free(current_dir);
	return (true);
}
