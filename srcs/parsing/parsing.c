/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:08:32 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/13 14:48:57 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <ms_define.h>
#include <parsing_ms.h>
#include <minishell.h>
#include <libft.h>
#include <errno.h>
#include <stdio.h>

bool	check_and_store_delimiter(char *str, int *storage)
{
	if (str[0] == '|')
	{
		if (str[1] == '|')
			*storage = OR_OPERATOR;
		else
			*storage = PIPE_OPERATOR;
	}
	else if (str[0] == '&' && str[1] == '&')
		*storage = AND_OPERATOR;
	else if (str[0] == ';')
		*storage = SEMI_COLON;
	if (*storage)
		return (true);
	return (false);
}

bool	is_valid_param(t_split_arg *param, int type, t_block *block)
{
	// errno = 0;
	if (type == -1)
		return (false);
	else if (type == CMD_ARG && !block->sub)
		ft_addargs(&block->cmd.args, param);
	else if (type == INPUT_OUTPUT)
		ft_add_io(block, param);
	else if (type == PARENTHESIS && !block->cmd.name && !block->sub)
	{
		block->subshell_command = param->str;
		free(param);
		add_block_back(&block, last_sub);
	}
	else
		return (false);
	if (errno)
		return (false);
	return (true);
}

t_split_arg	*get_next_param(char *str, int *i, int *type)
{
	char	*res;
	t_split_arg	*arg = NULL;

	res = NULL;
	*i += pass_whitespaces(&str[*i]);
	if (!str[*i])
		return (NULL);
	if (check_parenthesis_param(str, i, &res, type) \
		|| check_io_param(str, i, &res, type) \
		|| check_word_param(str, i, type, &arg))
		return (arg);
	*type = -1;
	return (NULL);
}

void	ft_error(int err, char *comment)
{
	ft_putstr_fd("minishell: ", 2);
	if (err == CMD_SYNTAX_ERR)
	{
		ft_putstr_fd("syntax error near unexpected token ", 2);
		ft_putchar_fd('`', 2);
		ft_putstr_fd(comment, 2);
		ft_putstr_fd("`\n", 2);
	}
}

bool	parse_cmds(t_block **curr_block, char *cmd_line)
{
	int		i;
	int		type;
	t_split_arg	*next_param;

	i = 0;
	type = -1;
	if (!*curr_block || !cmd_line)
		return (false);
	next_param = get_next_param(cmd_line, &i, &type);
	while (next_param && is_valid_param(next_param, type, *curr_block))
	{
		i += pass_whitespaces(&cmd_line[i]);
		if (type == PARENTHESIS)
		{
			if (!parse_cmds(&((*curr_block)->sub), next_param->str))
				return (free(cmd_line), free(next_param), false);
		}
		if (check_and_store_delimiter(&cmd_line[i], &(*curr_block)->operator))
		{
			i += pass_ws_and_delim(&cmd_line[i], (*curr_block)->operator);
			if ((*curr_block)->operator == PIPE_OPERATOR)
			{
				curr_block = &(*curr_block)->pipe_next;
				add_block_back(curr_block, last_pipe);
			}
			else
			{
				add_block_back(curr_block, last_sibling);
				curr_block = &(*curr_block)->next;
			}
		}
		next_param = get_next_param(cmd_line, &i, &type);
	}
	if (errno)
		return(free(cmd_line), free(next_param), false);
	if (next_param)
	{
		ft_error(CMD_SYNTAX_ERR, next_param->str/* provisoire doit etre remplace par un join */);
		free(next_param);
		return (false);
	}
	free(cmd_line);
	return (true);
}
