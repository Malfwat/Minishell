/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:08:32 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/18 06:18:24 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <ms_define.h>
#include <parsing_ms.h>
#include <minishell.h>
#include <env_function.h>
#include <libft.h>
#include <errno.h>
#include <stdio.h>

bool	is_valid_param(void *param, int type, t_block *block)
{
	if (type < 0)
		return (false);
	else if (type == CMD_ARG && !block->sub)
		ft_ls_t_args_addback(&block->cmd.args, param);
	else if (type == INPUT_OUTPUT)
		ft_add_io(block, param);
	else if (type == PARENTHESIS && !block->cmd.args && !block->sub)
	{
		if (is_parenthesis_empty((char *)param))
			return (false);
		block->subshell_command = (char *)param;
		add_block_back(&block, last_sub);
	}
	else
		return (false);
	if (errno)
		return (false);
	return (true);
}

void	*get_next_param(char *str, int *i, int *type)
{
	void	*res;

	res = NULL;
	*i += pass_whitespaces(&str[*i]);
	if (!str[*i])
		return (NULL);
	if ((!errno && check_parenthesis_param(str, i, (char **)&res, type)) \
		|| (!errno && check_io_param(str, i, type,  (t_split_arg **)&res)) \
		|| (!errno && check_word_param(str, i, type, (t_split_arg **)&res)))
		return (res);
	return (res);
}

bool	check_parse_error(void *next_param, char *cmd_line, int type)
{
	char	*tmp;

	if (errno)
		return(free(cmd_line), free_next_param(&next_param, type), false);
	if (next_param || type < 0)
	{
		tmp = ft_strtrim(cmd_line, " \t");
		if (ft_strlen(tmp) == 0)
			return (free(tmp), free(cmd_line), false);
		syntax_error(CMD_SYNTAX_ERR, next_param, type, tmp);
		free(tmp);
		free(cmd_line);
		free_next_param(&next_param, type);
		return (false);
	}
	free(cmd_line);
	return (true);
}

int	manage_delim(t_block **curr_block, char *cmd_line, int *i)
{
	*i += pass_ws_and_delim(&cmd_line[*i], (*curr_block)->operator);
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
	return (EXPECTING_ARGUMENT);
}

bool	parse_cmds(t_block **curr_block, char *cmd_line)
{
	int		i;
	int		type;
	void	*next_param;

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
			if (!parse_cmds(&((*curr_block)->sub), ft_substr(next_param, 1, \
			ft_strlen(next_param) - 2)))
				return (free(cmd_line), false);
		}
		if (check_and_store_delimiter(&cmd_line[i], &(*curr_block)->operator))
			type = manage_delim(curr_block, cmd_line, &i);
		next_param = get_next_param(cmd_line, &i, &type);
	}
	return (check_parse_error(next_param, cmd_line, type));
}
