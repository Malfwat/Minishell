/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 06:07:08 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 03:23:35 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <parsing_ms.h>
#include <ms_define.h>
#include <minishell.h>
#include <libft.h>

char	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (c);
	else
		return (0);
}

bool	is_delim(char *str)
{
	if (str && (ft_strchr(SINGLE_DELIM, *str) \
		|| ft_strschr_here(4, str, "&&", "||", "<<", ">>")))
		return (true);
	return (false);
}

static bool	must_break(char *src, int i, char quotes)
{
	if (((is_delim(&src[i]) || ft_strchr("()", src[i])) && !quotes) \
	|| (quotes && src[i] == quotes))
		return (true);
	else if (!quotes \
		&& ft_strchr("'\"", src[i]) && ft_strchr(&src[i + 1], src[i]))
		return (true);
	return (false);
}

int	slice_next_part(char *src, t_s_arg **last_args, char quotes)
{
	int		i;
	char	*sliced;
	t_s_arg	*new;

	i = -1;
	while (src[++i])
	{
		if (must_break(src, i, quotes))
			break ;
	}
	if (quotes && i == 0)
		sliced = ft_strdup("");
	else if (!quotes && i == 0)
		return (errno = 1, 0);
	else
		sliced = ft_substr(src, 0, i);
	new = ls_split_args_new(sliced, quotes);
	if (!new)
		return (0);
	if (src[i] && !is_delim(&src[i]) && (src[i] == quotes \
		|| !ft_strchr("()\'\"", src[i])))
		i++;
	return (ls_split_args_addback(last_args, new), i);
}

void	free_next_param(void **ptr, int type)
{
	if (type == PARENTHESIS || type == INCOMPLETE_PARENTHESIS)
		free(*ptr);
	else
		free_t_s_arg((t_s_arg **)ptr);
	*ptr = NULL;
}
