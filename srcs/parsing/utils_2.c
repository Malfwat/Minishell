/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 06:14:20 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/18 06:21:49 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_define.h>
#include <stdbool.h>
#include <struct_ms.h>

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
