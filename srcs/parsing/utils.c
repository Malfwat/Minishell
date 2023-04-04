/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 21:11:04 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/04 21:30:24 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
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
	if (!count)
		count = 2;
	return (count);
}

int	copy_chars_inside_quotes(char *src, char c, char **dest)
{
	int	i;

	i = 0;
	if (src[i] == c)
	{
		*((*dest)++) = c;
		*((*dest)++) = c;
		return (1);
	}
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

int	pass_ws_and_delim(char *str, int type)
{
	int	i;

	i = 0;
	while (ft_strchr(" \t", str[i]))
		i++;
	if (type < 2)
		return (i + 2);
	return (i + 1);
}
