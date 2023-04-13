/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:08:32 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/13 17:28:45 by hateisse         ###   ########.fr       */
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

bool	is_valid_param(void *param, int type, t_block *block)
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
		block->subshell_command = (char *)param;
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
	void	*res;

	res = NULL;
	*i += pass_whitespaces(&str[*i]);
	if (!str[*i])
		return (NULL);
	if (check_parenthesis_param(str, i, &res, type) \
		|| check_io_param(str, i, &res, type) \
		|| check_word_param(str, i, type, &res))
		return (res);
	*type = -1;
	return (NULL);
}

char	*ft_join_splitted_arg(t_split_arg *arg)
{
	char	*res;
	char	*tmp;

	res = NULL;
	while (arg)
	{
		tmp = res;
		res = ft_strjoin(res, arg->str);
		free(tmp);
		arg = arg->next;
	}	
}

void	ft_error(int err, void *comment, int type)
{
	char	*io_or_cmd_arg_case;

	if (type == CMD_ARG || type == INPUT_OUTPUT)
		io_or_cmd_arg_case = ft_join_splitted_arg()
	ft_putstr_fd("minishell: ", 2);
	if (err == CMD_SYNTAX_ERR)
	{
		ft_putstr_fd("syntax error near unexpected token ", 2);
		ft_putchar_fd('`', 2);
		ft_putstr_fd(comment, 2);
		ft_putstr_fd("`\n", 2);
	}
}

void	free_split_arg(t_split_arg *lst)
{
	t_split_arg	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->str);
		free(lst);
		lst = tmp;
	}
}

void	free_next_param(void *ptr, int type)
{
	if (type == PARENTHESIS)
		free(ptr);
	else
		free_split_arg(ptr);
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
		ft_error(CMD_SYNTAX_ERR, next_param, type);
		free(next_param);
		return (false);
	}
	free(cmd_line);
	return (true);
}
