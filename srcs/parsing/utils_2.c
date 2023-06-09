/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 06:14:20 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/10 18:27:04 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_define.h>
#include <stdbool.h>
#include <libft.h>
#include <minishell.h>
#include <ms_parsing.h>
#include <stdlib.h>
#include <ms_struct.h>

void	print_heredoc_syntax_error(char *heredoc_delimiter)
{
	ft_putstr_fd("minishell: heredoc: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted ", 2);
	ft_putchar_fd('`', 2);
	if (heredoc_delimiter)
		ft_putstr_fd(heredoc_delimiter, 2);
	ft_putstr_fd("`)\n", 2);
}

void	print_syntax_error(int type, char *str)
{
	char	c;

	if (type == ILLEGAL_HEREDOC)
		str = ft_strchr(str, '<') + 2;
	else if (type == ILLEGAL_OUTPUT)
		str = ft_strchr(str, '>') + 1;
	else if (type == ILLEGAL_AOUTPUT)
		str = ft_strchr(str, '>') + 2;
	else if (type == ILLEGAL_INPUT)
		str = ft_strchr(str, '<') + 1;
	str += pass_whitespaces(str);
	c = *str;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token ", 2);
	ft_putchar_fd('`', 2);
	if (c == '\n' || c == 0)
		ft_putstr_fd("newline", 2);
	else
		ft_putchar_fd(c, 2);
	ft_putstr_fd("`\n", 2);
}

void	syntax_error(int err, void *comment, int type, char *cmd_line)
{
	if (type == EXPECTING_ARGUMENT)
	{
		ms_perror("minishell", "syntax error", \
		"right hand operand cannot be empty");
		return ;
	}
	else if (!comment)
	{
		if (type <= ILLEGAL_HEREDOC)
			print_syntax_error(type, cmd_line);
		else
			print_syntax_error(type, cmd_line);
		return ;
	}
	else if (type == CMD_ARG || type == INPUT_OUTPUT)
	{
		comment = join_splitted_arg_syntax((t_s_arg *)comment, NULL, false);
		if (!comment)
			return ;
	}
	if (err == CMD_SYNTAX_ERR)
		print_syntax_error(type, (char *)comment);
	if (type == CMD_ARG || type == INPUT_OUTPUT)
		free(comment);
}

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
	else
		return (false);
	return (true);
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
