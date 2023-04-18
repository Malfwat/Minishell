/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_io_params.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:47:23 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/18 22:02:58 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <struct_ms.h>
#include <errno.h>
#include <minishell.h>
#include <parsing_ms.h>
#include <ms_define.h>

t_redirect	*new_redirect(t_split_arg *arg, int mode)
{
	t_redirect	*new;

	new = ft_calloc(1, sizeof(t_redirect));
	if (!new)
		return (free_t_split_arg(&arg), NULL);
	if (!ft_strncmp(arg->str, "<<", 2))
		new->hd_lim = arg;
	else if (arg->str[0] == '<')
		new->file_name = arg;
	else if (!ft_strncmp(arg->str, ">>", 2))
	{
		new->append = true;
		new->file_name = arg;
	}
	else
		new->file_name = arg;
	new->mode = mode;
	new->fd = INIT_FD_VALUE;
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

void	ft_add_redirect(t_redirect **head, t_split_arg *arg, int mode)
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

void	ft_add_io(t_block *block, t_split_arg *io)
{
	if (!ft_strncmp(io->str, "<<", 2))
	{
		block->input_source = HEREDOC;
		ft_add_redirect(&block->heredoc, io, INPUT_MODE);
	}
	else if (io->str[0] == '<')
	{
		block->input_source = FILE_INPUT;
		ft_add_redirect(&block->io_redirect, io, INPUT_MODE);
	}
	else
		ft_add_redirect(&block->io_redirect, io, OUTPUT_MODE);
}

bool	check_io_param(char *str, int *i, int *type, t_split_arg **arg)
{
	char		quotes;
	char		redirect[3];

	*type = INCOMPLETE_INPUT_OUTPUT;
	*i += pass_whitespaces(&str[*i]);
	ft_bzero(redirect, 3);
	if (str[*i] && ft_strchr("><", str[*i]))
	{
		redirect[0] = str[*i];
		if (str[*i] == str[*i + 1])
			redirect[1] = str[(*i)++];
		*arg = ls_split_args_new(ft_strdup(redirect), 0);
		if (!(*arg))
			return (true);
		(*i)++;
		*i += pass_whitespaces(&str[*i]);
		while (str[*i] && !ft_strchr("><", str[*i]) && !is_delim(&str[*i]))
		{
			quotes = 0;
			if (ft_strchr("'\"", str[*i]) && ft_strchr(&str[*i + 1], str[*i]))
			{
				quotes = str[*i];
				(*i)++;
			}
			(*i) += slice_next_part(&str[*i], arg, quotes);
			if (quotes)
				(*i)++;
		}
		if (*arg && !errno)
		{
			*type = INPUT_OUTPUT;
			return (true);
		}
	}
	return (false);
}