/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 06:25:37 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:16:13 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_parsing.h>
#include <libft.h>
#include <errno.h>
#include <fcntl.h>
#include <ms_define.h>
#include <minishell.h>
#include <stdio.h>
#include <ms_struct.h>

int	input_manager(t_redirect *ptr, t_fd *fd, t_block *block, t_env *envp)
{
	ptr->joined_name = join_splitted_arg(ptr->file_name->next, envp, true);
	if (errno)
		return (-1);
	if (ptr->joined_name)
		ptr->fd = open(ptr->joined_name, O_RDONLY);
	if (*fd > 2)
	{
		close(*fd);
		*fd = INIT_FD_VALUE;
	}
	if (ptr->fd != -1)
	{
		if (block->input_source == FILE_INPUT)
			*fd = ptr->fd;
	}
	ptr->errno_value = errno;
	if (errno)
		return (-2);
	return (0);
}

int	output_manager(t_redirect *ptr, t_fd *fd, t_env *envp)
{
	ptr->joined_name = join_splitted_arg(ptr->file_name->next, envp, true);
	if (errno)
		return (-1);
	if (ptr->append)
		ptr->fd = open(ptr->joined_name, O_WRONLY | O_CREAT | O_APPEND, 00644);
	else
		ptr->fd = open(ptr->joined_name, O_WRONLY | O_TRUNC | O_CREAT, 00644);
	if (*fd > 2)
	{
		close(*fd);
		*fd = INIT_FD_VALUE;
	}
	if (ptr->fd != -1)
	{
		*fd = ptr->fd;
	}
	ptr->errno_value = errno;
	if (errno)
		return (-2);
	return (0);
}
