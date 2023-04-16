/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:08:32 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/16 20:44:34 by hateisse         ###   ########.fr       */
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

bool	is_parenthesis_empty(char *str)
{
	int	i;

	i = 1;
	i += pass_whitespaces(&str[i]);
	if (str[i] == ')' && !str[i + 1])
		return (true);
	return (false);
}

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

char	*interpret_dollars(t_split_arg *arg, t_env_var *envp)
{
	char		*tmp;
	char		*res;
	t_env_var	*env_var;
	char		**tab;
	int			i;

	tab = ft_split(arg->str, '$');
	if (!tab)
		return (NULL);
	i = -1;
	res = NULL;
	while (tab[++i] && !errno)
	{
		tmp = res;
		if ((i == 0 && arg->str[0] != '$') || (arg->scope == '\''))
			res = ft_strjoin(res, tab[i]);
		else
		{
			env_var = find_env_var(envp, tab[i]);
			if (!env_var)
				res = ft_strjoin(res, "");
			else
				res = ft_strjoin(res, env_var->var_value);
		}
		free(tmp);
	}
	return (ft_strsfree(tab), res);
}

void	free_t_split_arg(t_split_arg **arg)
{
	t_split_arg	*ptr;

	while (*arg)
	{
		ptr = (*arg)->next;
		free((*arg)->str);
		free(*arg);
		*arg = ptr;
	}
	*arg = NULL;
}

char	*join_splitted_arg(t_split_arg *arg, t_env_var *envp, bool interpret)
{
	char	*res;
	char	*tmp;
	char	*dollar_interpreted;

	res = NULL;
	dollar_interpreted = NULL;
	while (arg)
	{
		tmp = res;
		if (interpret)
		{
			dollar_interpreted = interpret_dollars(arg, envp);
			res = ft_strjoin(res, dollar_interpreted);
			free(dollar_interpreted);
		}
		else
			res = ft_strjoin(res, arg->str);
		free(tmp);
		if (errno)
			return (free(res), NULL);
		arg = arg->next;
	}
	return (res);
}

void	print_syntax_error(char c)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token ", 2);
	ft_putchar_fd('`', 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("`\n", 2);
}

void	syntax_error(int err, void *comment, int type, char *cmd_line)
{
	if (type == EXPECTING_ARGUMENT)
	{
		ft_putstr_fd("minishell: syntax error: right hand operand cannot be empty\n", 2);
		return ;
	}
	else if (!comment)
	{
		print_syntax_error(*cmd_line);
		return ;
	}
	else if (type == CMD_ARG || type == INPUT_OUTPUT)
	{
		comment = join_splitted_arg((t_split_arg *)comment, NULL, false);
		if (!comment)
			return ;
	}
	if (err == CMD_SYNTAX_ERR)
		print_syntax_error(*(char *)comment);
	if (type == CMD_ARG || type == INPUT_OUTPUT)
		free(comment);
}

void	free_split_args(t_split_arg *lst)
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

void	free_next_param(void **ptr, int type)
{
	if (type == PARENTHESIS || type == INCOMPLETE_PARENTHESIS)
		free(*ptr);
	else
		free_split_args(*ptr);
	*ptr = NULL;
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
			type = EXPECTING_ARGUMENT;
		}
		next_param = get_next_param(cmd_line, &i, &type);
	}
	return (check_parse_error(next_param, cmd_line, type));
}
