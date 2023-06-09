/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:08:33 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:16:13 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdbool.h>
#include <ms_parsing.h>
#include <ms_define.h>

int	find_closing_parenthesis(char *str)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '(')
			counter += 1;
		else if (str[i] == ')')
			counter -= 1;
		if (counter == 0)
			return (i);
		i += 1;
	}
	return (-1);
}

bool	check_parenthesis_param(char *str, int *i, char **new_line, int *type)
{
	int	start;

	start = *i;
	*type = INCOMPLETE_PARENTHESIS;
	if (str[*i] == '(')
	{
		*i += find_closing_parenthesis(&str[*i]);
		if (*i == -1)
		{
			*i = start;
			return (false);
		}
		*new_line = ft_substr(str, start, *i - start + 1);
		*type = PARENTHESIS;
		*i += 1;
		return (true);
	}
	return (false);
}
