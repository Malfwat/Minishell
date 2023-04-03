/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/03 21:01:29 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <libft.h>
#include <stdbool.h>
#include <stdlib.h>
#include <minishell.h>
#include <unistd.h>

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

int	ft_substr_word_param(char *src, char *dest, int length)
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
	// char	*str;

	size = 0;
	j = 0;
	if (!ft_strchr("><", str[*i + j]))
	{
		while (ft_strchr(" \t", str[*i + j]))
			j++;
		if (!count_param_length(&str[*i + j], SINGLE_DELIM, &size))
			return (false);
		*new_line = malloc((size + 1) * sizeof(char));
		*i += ft_substr_word_param(&str[*i], *new_line, size);
		return (true);
	}
	return (false);
}


int	ft_substr_io_param(char *src, char *dest, int length)
{
	int	i;

	i = 0;
	if (!dest)
		return (0);
	while (ft_strchr("><", src[i]))
		*(dest++) = src[i++];
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

bool	check_io_param(char *str, int *i, char **new_line)
{
	int		j;
	int		size;
	// char	*str;

	size = 1;
	j = 0;
	if (ft_strchr("><", str[*i + j++]))
	{
		if (str[*i + j] == str[*i])
		{
			j++;
			size++;
		}
		while (ft_strchr(" \t", str[*i + j]))
			j++;
		if (!count_param_length(&str[*i + j], SINGLE_DELIM, &size))
			return (false);
		*new_line = malloc((size + 1) * sizeof(char));
		*i += ft_substr_io_param(&str[*i], *new_line, size);
		return (true);
	}
	return (false);
}

int	main(int ac, char **av)
{
	char	*res = NULL;
	int		i = 0;
	
	if (ac != 2)
		return (0);
	check_io_param(av[1], &i, &res);
	printf("[%d] res: '%s'\n", i, res);
	// printf("%s\n", res);
}
