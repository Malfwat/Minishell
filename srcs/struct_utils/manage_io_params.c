/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_io_params.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:47:23 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/10 19:59:49 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <struct_ms.h>
#include <errno.h>
#include <parsing_ms.h>
#include <ms_define.h>

t_redirect	*new_redirect(char *arg, int mode)
{
	t_redirect	*new;

	new = ft_calloc(1, sizeof(t_redirect));
	if (!new)
		return (free(arg), NULL);
	if (!ft_strncmp(arg, "<<", 2))
		new->heredoc = ft_strdup(arg + 2);
	else if (arg[0] == '<')
		new->file_name = ft_strdup(arg + 1);
	else if (!ft_strncmp(arg, ">>", 2))
	{
		new->append = true;
		new->file_name = ft_strdup(arg + 2);
	}
	else
		new->file_name = ft_strdup(arg + 1);
	new->mode = mode;
	new->fd = INIT_FD_VALUE;
	free(arg);
	if (errno)
		return (NULL);
	return (new);
}

t_redirect	*last_redirect(t_redirect *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

void	ft_add_redirect(t_redirect **head, char *arg, int mode)
{
	t_redirect	*new;

	new = new_redirect(arg, mode);
	if (!new)
		return ;
	if (*head == NULL)
		*head = new;
	else
		last_redirect(*head)->next = new;
}

void	ft_add_io(t_block *block, char *io)
{
	if (!ft_strncmp(io, "<<", 2))
	{
		block->input_source = HEREDOC;
		ft_add_redirect(&block->heredoc, io, INPUT_MODE);
	}
	else if (io[0] == '<')
	{
		block->input_source = FILE_INPUT;
		ft_add_redirect(&block->io_redirect, io, INPUT_MODE);
	}
	else
		ft_add_redirect(&block->io_redirect, io, OUTPUT_MODE);
}
