/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 06:25:37 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 21:11:47 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_ms.h>
#include <libft.h>
#include <errno.h>
#include <fcntl.h>
#include <ms_define.h>
#include <minishell.h>
#include <stdio.h>
#include <struct_ms.h>

int	heredoc(char *limiter)
{
	int		tube[2];
	char	*str;
	char	*tmp;

	if (pipe(tube) == -1)
		return (-1);
	get_next_line(0, &str);
	tmp = ft_strjoin(limiter, "\n");
	while (str && ft_strcmp(str, tmp) && !errno)
	{
		write(tube[1], str, ft_strlen(str));
		free(str);
		get_next_line(0, &str);
	}
	if (!str)
		print_heredoc_syntax_error(limiter);
	free(str);
	free(tmp);
	my_close(tube[1], -2);
	return (tube[0]);
}

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

void	change_input_fd(t_block *block, t_fd *fd)
{
	if (block->input_source == HEREDOC)
	{
		if (block->io_tab[0] > 2)
			close(block->io_tab[0]);
		block->io_tab[0] = *fd;
	}
	else
	{
		my_close(*fd, -2);
		*fd = INIT_FD_VALUE;
	}
}

int	hd_manager(t_block *block)
{
	t_redirect	*ptr;

	if (!block)
		return (0);
	ptr = block->heredoc;
	while (ptr)
	{
		if (!ptr->hd_lim)
			return (0);
		ptr->joined_name = join_splitted_arg(ptr->hd_lim->next, NULL, false);
		if (errno)
			return (-1);
		ptr->fd = heredoc(ptr->joined_name);
		if (ptr->fd == -1)
			return (-1);
		change_input_fd(block, &ptr->fd);
		ptr = ptr->next;
	}
	if (hd_manager(block->pipe_next) == -1 \
		|| hd_manager(block->next) == -1 || hd_manager(block->sub) == -1)
		return (-1);
	return (0);
}
