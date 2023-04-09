/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 06:25:37 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/09 22:20:11 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_ms.h>
#include <libft.h>
#include <get_next_line.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <env_function.h>
#include <libft.h>
#include <struct_ms.h>

int	heredoc(char *limiter)
{
	int		test[2];
	char	*str;
	int		size;
	char	*tmp;

	if (pipe(test) == -1)
		return (-1);
	get_next_line(0, &str);
	size = ft_strlen(str);
	tmp = ft_strjoin(limiter, "\n");
	while (ft_strncmp(str, tmp, size) && !errno)
	{
		write(test[1], str, size);
		free(str);
		get_next_line(0, &str);
		size = ft_strlen(str);
	}
	free(str);
	free(tmp);
	close(test[1]);
	return (test[0]);
}

void	input_manager(t_redirect *ptr, int *fd)
{
	if (ptr->file_name)
		ptr->fd = open(ptr->file_name, O_RDONLY);
	if (ptr->fd != -1)
		*fd = ptr->fd;
	ptr->errno_value = errno;
}

void	output_manager(t_redirect *ptr, int *fd)
{
	if (ptr->append)
		ptr->fd = open(ptr->file_name, O_WRONLY | O_CREAT | O_APPEND, 00644);
	else
		ptr->fd = open(ptr->file_name, O_WRONLY | O_TRUNC | O_CREAT, 00644);
	if (ptr->fd != -1)
		*fd = ptr->fd;
	ptr->errno_value = errno;
}

bool	heredoc_manager(t_redirect *ptr)
{
	while (ptr)
	{
		ptr->fd = heredoc(ptr->heredoc);
		if (ptr->fd == -1)
			return (false);
		ptr = ptr->next;
	}
	return (true);
}

t_block *find_next_block(t_block *block, bool ignore_sub);


bool	io_manager(t_block *block)
{
	t_redirect	*tmp;

	if (!heredoc_manager(block->heredoc))
		return (false);
	while (block)
	{
		tmp = block->io_redirect;
		while (tmp && !errno)
		{
			if (tmp->mode == INPUT_MODE)
				input_manager(tmp, &block->io_tab[0]);
			else if (tmp->mode == OUTPUT_MODE)
				output_manager(tmp, &block->io_tab[1]);
			tmp = tmp->next;
		}
		if (block->sub)
			io_manager(block->sub);
		block = find_next_block(block, false);
	}
	if (errno)
		return (NULL);
	return (true);
}
