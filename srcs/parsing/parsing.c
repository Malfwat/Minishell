/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:08:32 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/04 22:19:08 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <parsing.h>
#include <minishell.h>
#include <libft.h>
#include <stdio.h>

bool	is_delimiter(char *str, int *storage)
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
	if (*storage != -1)
		return (true);
	return (false);
}

bool	is_valid_param(char *param, int type, t_block *block)
{
	errno = 0;
	if (type == -1)
		return (0);
	else if (!block->cmd.name && type == CMD_ARG && !block->sub)
		block->cmd.name = param;
	else if (block->cmd.name && type == CMD_ARG && !block->sub)
		ft_addargs(&block->cmd.args, param);
	else if (type == INPUT_OUTPUT)
		ft_add_io(block, param);
	else if (type == PARENTHESIS && !block->cmd.args && !block->sub)
	{
		block->subshell_command = param;
		add_block_back(&block->sub, &last_sub);
	}
	else
		return (0);
	if (errno)
		return (0);
	return (1);
}

char	*get_next_param(char *str, int *i, int *type)
{
	char	*res;

	res = NULL;
	if (check_parenthesis_param(str, i, &res, type) \
		|| check_io_param(str, i, &res, type) \
		|| check_word_param(str, i, &res, type))
		return (res);
	return (NULL);
}

void	ft_error(int err, char *comment)
{
	ft_putstr_fd("minishell: ", 2);
	if (err == CMD_SYNTAX_ERR)
	{
		ft_putstr_fd("syntax error near unexpected token \n", 2);
		ft_putchar_fd("`", 2);
		ft_putstr_fd(comment, 2);
		ft_putchar_fd("`\n", 2);
	}
}

bool	parse_cmd(t_block **curr_block, char *cmd_line)
{
	int		i;
	int		type;
	char	*next_param;

	i = 0;
	type = -1;
	next_param = get_next_param(cmd_line, &i, &type);
	while (next_param && is_valid_param(next_param, type, *curr_block))
	{
		if (type == PARENTHESIS)
		{
			if (parse_cmd(&((*curr_block)->sub), ft_substr(&cmd_line[i], 1, \
			ft_strlen(&cmd_line[i]) - 2)))
				return (free(cmd_line), false);
			next_param = get_next_param(cmd_line, &i, &type);
			continue ;
		}
		if (check_and_store_delimiter(cmd_line, &(*curr_block)->operator))
		{
			(*curr_block)->operator = is_delimiter(cmd_line, i);
			add_block_back(curr_block, last_sibling);
			curr_block = &(*curr_block)->next;
			i += pass_ws_and_delim(&cmd_line[i], (*curr_block)->operator);
		}
		next_param = get_next_param(cmd_line, &i, &type);
	}
	if (errno)
		return(free(cmd_line), free(next_param), \
		perror("minishell"), false);
	if (next_param)
	{
		ft_error(CMD_SYNTAX_ERR, next_param);
		free(next_param);
		return (false);
	}
	free(cmd_line);
	return (true);
}

if (next_param || errno)
	error