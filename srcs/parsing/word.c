/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:14:21 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:16:13 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_parsing.h>
#include <stdbool.h>
#include <ms_define.h>
#include <minishell.h>
#include <libft.h>

bool	check_word_param(char *str, int *i, int *type, t_s_arg	**arg)
{
	char		quotes;

	*type = INCOMPLETE_CMD_ARG;
	*i += pass_whitespaces(&str[*i]);
	while (str[*i] && !is_delim(&str[*i]) && !ft_strchr("()", str[*i]))
	{
		quotes = 0;
		if (ft_strchr("'\"", str[*i]) && ft_strchr(&str[*i + 1], str[*i]))
		{
			quotes = str[*i];
			(*i)++;
		}
		(*i) += slice_next_part(&str[*i], arg, quotes);
		if (errno)
			return (false);
	}
	if (*arg && !errno)
	{
		*type = CMD_ARG;
		return (true);
	}
	return (false);
}
