/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:14:21 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 01:20:34 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>
#include <stdbool.h>
#include <libft.h>

int	ft_substr_word_param(char *src, char *dest)
{
	int	i;

	i = 0;
	if (!dest)
		return (0);
	while (ft_strchr(" \t", src[i]))
		i++;
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

bool	check_word_param(char *str, int *i, char **new_line)
{
	int		j;
	int		size;

	size = 0;
	j = 0;
	if (!ft_strchr("><", str[*i + j]))
	{
		while (ft_strchr(" \t", str[*i + j]))
			j++;
		if (!count_param_length(&str[*i + j], SINGLE_DELIM, &size))
			return (false);
		*new_line = malloc((size + 1) * sizeof(char));
		*i += ft_substr_word_param(&str[*i], *new_line);
		return (true);
	}
	return (false);
}
