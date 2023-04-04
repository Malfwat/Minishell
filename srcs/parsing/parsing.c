/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:08:32 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/04 15:29:21 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct.h>
#include <parsing.h>
#include <minishell.h>
#include <libft.h>
#include <stdio.h>

int	count_chars_inside_quotes(char **str, char c)
{
	int	count;
	
	count = 0;
	(*str)++;
	while (**str && **str != c)
	{
		count++;
		(*str)++;
	}
	if (**str)
		(*str)++;
	return (count);
}

int	copy_chars_inside_quotes(char *src, char c, char **dest)
{
	int	i;

	i = 0;
	while (src && src[i] != c)
		*((*dest)++) = src[i++];
	if (src[i])
		i++;
	return (i);
}

int	count_param_length(char *str, char *charset, int *size)
{
	int	count;

	count = 0;
	while (*str && !ft_strchr(charset, *str)
		&& !ft_strschr_here(4, str, "&&", "||", "<<", ">>"))
	{
		if (ft_strchr("'\"", *str) && ft_strchr(str + 1, *str))
			count += count_chars_inside_quotes(&str, *str);
		else
		{
			count++;
			str++;
		}
	}
	if (size)
		*size += count;
	return (count);
}

char	*get_next_param(char *str, int *i, int *type)
{
	char	*res;

	res = NULL;
	if (check_parenthesis_param(str, i, &res, type) \
		|| check_io_param(str, i, &res, type) \
		|| check_word_param(str, i, &res, type))
	{
		if (res == NULL)
			return (perror("malloc failed"), NULL);
	}
	return (res);
}