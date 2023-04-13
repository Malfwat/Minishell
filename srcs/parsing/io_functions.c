/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 06:25:37 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/13 18:31:20 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_ms.h>
#include <libft.h>
#include <errno.h>
#include <fcntl.h>
#include <ms_define.h>
// #include <env_function.h>
#include <stdio.h>
#include <struct_ms.h>

int	heredoc(char *limiter)
{
	int		test[2];
	char	*str;
	char	*tmp;

	if (pipe(test) == -1)
		return (-1);
	get_next_line(0, &str);
	tmp = ft_strjoin(limiter, "\n");
	while (ft_strcmp(str, tmp) && !errno)
	{
		write(test[1], str, ft_strlen(str));
		free(str);
		get_next_line(0, &str);
	}
	free(str);
	free(tmp);
	close(test[1]);
	return (test[0]);
}

void	input_manager(t_redirect *ptr, t_fd *fd, t_block *block)
{
	if (ptr->file_name)
		ptr->fd = open(ptr->file_name, O_RDONLY);
	if (ptr->fd != -1)
	{
		if (block->input_source == FILE_INPUT)
		{
			if (*fd != INIT_FD_VALUE)
				close(*fd);
			*fd = ptr->fd;
		}
	}
	ptr->errno_value = errno;
}

void	output_manager(t_redirect *ptr, t_fd *fd)
{
	if (ptr->append)
		ptr->fd = open(ptr->file_name, O_WRONLY | O_CREAT | O_APPEND, 00644);
	else
		ptr->fd = open(ptr->file_name, O_WRONLY | O_TRUNC | O_CREAT, 00644);
	if (ptr->fd != -1)
	{
		if (*fd != INIT_FD_VALUE)
		{
			dprintf(2, "on close %d\n", *fd);
			close(*fd);
		}
		*fd = ptr->fd;
	}
	ptr->errno_value = errno;
}

bool	heredoc_manager(t_redirect *ptr, t_block *block)
{
	while (ptr)
	{
		ptr->fd = heredoc(ptr->heredoc);
		if (ptr->fd == -1)
			return (false);
		if (block->input_source == HEREDOC)
		{
			if (block->io_tab[0] != INIT_FD_VALUE)
				close(block->io_tab[0]);
			block->io_tab[0] = ptr->fd;
		}
		ptr = ptr->next;
	}
	return (true);
}

t_block *find_next_block(t_block *block, bool ignore_sub);


bool	io_manager(t_block *block)
{
	t_redirect	*tmp;

	if (!heredoc_manager(block->heredoc, block))
		return (false);
	while (block)
	{
		tmp = block->io_redirect;
		while (tmp && !errno)
		{
			if (tmp->mode == INPUT_MODE)
				input_manager(tmp, &block->io_tab[0], block);
			else if (tmp->mode == OUTPUT_MODE)
				output_manager(tmp, &block->io_tab[1]);
			tmp = tmp->next;
		}
		if (block->operator == PIPE_OPERATOR)
			io_manager(block->pipe_next);
			io_manager(block->sub);
		block = find_next_block(block, true);
	}
	if (errno)
		return (NULL);
	return (true);
}
