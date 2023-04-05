/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/05 16:05:05 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
// #include <libft.h>
#include <stdbool.h>
#include <stdlib.h>

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = -1;
	while (s[++i])
		if ((unsigned char)s[i] == (unsigned char)c)
			return ((char *)&s[i]);
	if (!s[i] && !c)
		return ((char *)&s[i]);
	return (0);
}

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
	while (*str && !ft_strchr(charset, *str))
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

	if (!dest)
		return (0);
	i = pass_whitespaces(src);
	while (src[i] && !ft_strchr(" \t><", src[i]))
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
	if (!ft_strchr("><", str[*i + j++]))
	{
		j += pass_whitespaces(&str[*i + j]);
		if (!count_param_length(&str[*i + j], " \t><", &size))
			return (false);
		*new_line = malloc((size + 1) * sizeof(char));
		*i += ft_substr_word_param(&str[*i], *new_line, size);
		return (true);
	}
	return (false);
}

#include <unistd.h>

// void	ft_putstr(char *str)
// {
// 	int	i;

// 	i = -1;
// 	while (str[++i])
// 		write(1, &str[i], 1);
// }

// int	main(int ac, char **av)
// {
// 	char	*res = NULL;
// 	int		i = 0;
	
// 	if (ac != 2)
// 		return (0);
// 	check_word_param(av[1], &i, &res);
// 	ft_putstr(((char *[]){ "test", "test2" })[0]);
// 	// printf("%s\n", res);
// }

#include "libft.h"

int main(int argc, char **argv) {


    // Accessing the char** created inline and passing it to ft_putstr
	if (!ft_strschr_here(8, "", "&&", "||", "|", ";", "<<", ">>", "<", ">"))
		printf("pas trouve\n");
	else
		printf("trouve\n");
	// int i = (int []){0, 25}[0];
	// printf("%d\n", i);
    return 0;
}