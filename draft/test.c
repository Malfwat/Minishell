/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:59:39 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/04 21:13:44 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <struct_ms.h>
#include <minishell.h>
#include <libft.h>
#include <errno.h>



void parse_cmd(t_block **curr_block, char *cmd_line)
{
	int i = 0;
	int	type = -1;
	char	*next_param;

	next_param = get_next_param(cmd_line, &i, &type);
	while (next_param && is_valid_param(next_param, type, *curr_block))
	{
		if (type == PARENTHESIS)
			if (func(&((*curr_block)->sub), ft_substr(&cmd_line[i], 1, \
			ft_strlen(&cmd_line[i]) - 2)) == -1)
				return (free(cmd_line), -1);
			next_param = get_next_param(cmd_line, &i, &type);
			continue ;
		if (check_and_store_delimiter(cmd_line, &(*curr_block)->operator))
		{
			(*curr_block)->operator = is_delimiter(cmd_line, i);
			add_block_back(curr_block, last_sibling);
			curr_block = &(*curr_block)->next;
			i += pass_ws_and_delim(&cmd_line[i], (*curr_block)->operator);
		}
		next_param = get_next_param(cmd_line, &i, &type);
	}
	if (next_param)
		error(free(next_param), -1);
	free(cmd_line);
	return (0);
}

(str && free(str));
( || return )

t_block *head = NULL;

char *line = change_dollar_var(rl_line_buffer);
func(&head, line);