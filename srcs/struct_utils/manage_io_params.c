/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_io_params.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:47:23 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/27 18:41:32 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <struct_ms.h>
#include <errno.h>
#include <minishell.h>
#include <parsing_ms.h>
#include <ms_define.h>

void	ft_add_redirect(t_redirect **head, t_s_arg *arg, int mode)
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

void	ft_add_io(t_block *block, t_s_arg *io)
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

static bool	must_return(char redirect[3], int *type, t_s_arg **arg)
{
	if (!ft_strcmp(redirect, "<<") && !(*arg)->next)
		return (free_t_s_arg(arg), *type = ILLEGAL_HEREDOC, true);
	else if (!ft_strcmp(redirect, "<") && !(*arg)->next)
		return (free_t_s_arg(arg), *type = ILLEGAL_INPUT, true);
	else if (!ft_strcmp(redirect, ">") && !(*arg)->next)
		return (free_t_s_arg(arg), *type = ILLEGAL_OUTPUT, true);
	else if (!ft_strcmp(redirect, ">>") && !(*arg)->next)
		return (free_t_s_arg(arg), *type = ILLEGAL_AOUTPUT, true);
	return (false);
}

void	get_redirect(char redirect[3], char *str, int *i)
{
	(redirect)[0] = str[*i];
	(redirect)[1] = 0;
	if (str[*i] == str[*i + 1])
		(redirect)[1] = str[(*i)++];
	(redirect)[2] = 0;
}

bool	check_io_param(char *str, int *i, int *type, t_s_arg **arg)
{
	char		quotes;
	char		redirect[3];

	*type = INCOMPLETE_INPUT_OUTPUT;
	*i += pass_whitespaces(&str[*i]);
	if (str[*i] && ft_strchr("><", str[*i]))
	{
		get_redirect(redirect, str, i);
		*arg = ls_split_args_new(ft_strdup(redirect), 0);
		if (!(*arg))
			return (true);
		*i += pass_whitespaces(&str[(*i) + 1]) + 1;
		while (str[*i] && !ft_strchr("><", str[*i]) && !is_delim(&str[*i]))
		{
			quotes = 0;
			if (ft_strchr("'\"", str[*i]) && ft_strchr(&str[*i + 1], str[*i]))
				quotes = str[(*i)++];
			(*i) += slice_next_part(&str[*i], arg, quotes);
		}
		if (must_return(redirect, type, arg))
			return (false);
		if (*arg && !errno)
			return (*type = INPUT_OUTPUT, true);
	}
	return (free_t_s_arg(arg), false);
}
