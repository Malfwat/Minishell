/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:08:33 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/06 19:48:55 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdbool.h>
#include <minishell.h>

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
	if (str[*i] == '(')
	{
		*i += find_closing_parenthesis(&str[*i]);
		if (*i == -1)
			return (false);
		*new_line = ft_substr(str, start, *i - start + 1);
		*type = PARENTHESIS;
		*i += 1;
		return (true);
	}
	return (false);
}
