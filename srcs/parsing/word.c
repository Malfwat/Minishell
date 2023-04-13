/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:14:21 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/13 16:39:09 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_ms.h>
#include <stdbool.h>
#include <ms_define.h>
#include <minishell.h>
#include <libft.h>

int	ft_substr_word_param(char *src, char *dest)
{
	int	i;

	if (!dest)
		return (0);
	i = pass_whitespaces(src);
	while (src[i] && !ft_strchr(SINGLE_DELIM, src[i])
		&& !ft_strschr_here(4, &src[i], "&&", "||", "<<", ">>"))
	{
		if (ft_strchr("'\"", src[i]) && ft_strchr(&src[i + 1], src[i]))
			i += 1 + copy_chars_inside_quotes(&src[i + 1], src[i], &dest);
		else
			*(dest++) = src[i++];
	}
	*(dest) = 0;
	return (i);
}

#include <stdio.h>

void	ls_split_arg_addback(t_split_arg **head, t_split_arg *new)
{
	t_split_arg	*tmp;

	if (!*head)
		*head = new;
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_split_arg	*ls_split_arg_new(char *data, char scope)
{
	t_split_arg	*new;

	if (!data)
		return (NULL);
	new = ft_calloc(1, sizeof(t_split_arg));
	if (!new)
		return (NULL);
	new->str = data;
	new->scope = scope;
	return (new);
}

char	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (c);
	else
		return (0);
}

bool	is_delim(char *str)
{
	if (*str && (ft_strchr(SINGLE_DELIM, *str)
		|| ft_strschr_here(4, str, "&&", "||", "<<", ">>")))
		return (true);
	return (false);
}

int	slice_next_part(char *src, t_split_arg **last_arg, char quotes)
{
	int			i;
	char		*sliced;
	t_split_arg	*new;

	i = 0;
	while (src[i] && !is_delim(&src[i]))
	{
		if (!quotes && ft_strchr("'\"", src[i]) && ft_strchr(&src[i + 1], src[i]))
			break ;
		else if (quotes && src[i] == quotes)
			break ;
		else
			i++;
	}
	if (quotes && i == 0)
		sliced = ft_strdup("");
	else if (!quotes && i == 0)
		return (0);
	else
		sliced = ft_substr(src, 0, i);
	new = ls_split_arg_new(sliced, quotes);
	ls_split_arg_addback(last_arg, new);
	return (i);
}

bool	check_word_param(char *str, int *i, char **new_line, int *type)
{
	t_split_arg	*arg;
	char		quotes;

	arg = NULL;
	(void)new_line;
	pass_whitespaces(&str[*i]);
	while (str[*i] && !ft_strchr("><", str[*i]))
	{
		quotes = 0;
		if (ft_strchr("'\"", str[*i]) && ft_strchr(&str[*i + 1], str[*i]))
		{
			quotes = str[*i];
			(*i)++;
		}
		*i += slice_next_part(&str[*i], &arg, quotes);
		if (quotes)
			(*i)++;
	}
	if (arg && errno)
	{
		*type = CMD_ARG;
		return (true);
	}
	return (false);
}

