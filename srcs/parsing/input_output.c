/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:11:59 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/13 17:04:22 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_ms.h>
#include <stdbool.h>
#include <ms_define.h>
// #include <minishell.h>
#include <libft.h>

// int	ft_substr_io_param(char *src, char *dest)
// {
// 	int	i;

// 	i = 0;
// 	if (!dest)
// 		return (0);
// 	while (src[i] && ft_strchr("><", src[i]))
// 		*(dest++) = src[i++];
// 	i += pass_whitespaces(&src[i]);
// 	while (src[i] && !ft_strchr(SINGLE_DELIM, src[i])
// 		&& !ft_strschr_here(4, &src[i], "&&", "||", "<<", ">>"))
// 	{
// 		if (ft_strchr("'\"", src[i]) && ft_strchr(&src[i + 1], src[i]))
// 			i += 1 + copy_chars_inside_quotes(&src[i + 1], src[i], &dest);
// 		else
// 			*(dest++) = src[i++];
// 	}
// 	*(dest) = 0;
// 	return (i);
// }

// bool	check_io_param(char *str, int *i, char **new_line, int *type)
// {
// 	int		j;
// 	int		size;

// 	size = 1;
// 	j = 0;
// 	if (ft_strchr("><", str[*i + j++]))
// 	{
// 		if (str[*i + j] == str[*i])
// 		{
// 			j++;
// 			size++;
// 		}
// 		j += pass_whitespaces(&str[*i + j]);
// 		if (!count_param_length(&str[*i + j], SINGLE_DELIM, &size))
// 			return (false);
// 		*new_line = malloc((size + 1) * sizeof(char));
// 		*i += ft_substr_io_param(&str[*i], *new_line);
// 		*type = INPUT_OUTPUT;
// 		return (true);
// 	}
// 	return (false);
// }

// bool	check_io_param(char *str, int *i, int *type, t_split_arg **arg)
// {
// 	char		quotes;
// 	char		redirect[2];
// 	// char		scope;

// 	// scope = *(ft_strchrnul("'\"", str[*i]));
// 	pass_whitespaces(&str[*i]);
// 	if (ft_strchr("><", str[*i]))
// 	{
// 		redirect[0] = str[*i];
// 		if (str[*i] == str[*i + 1])
// 			redirect[1] = str[*i++];
// 		*arg = ls_split_arg_new(redirect, str[i]);
// 		if (!(*arg))
// 			return (true);
// 		(*i)++;

		
// 		while (str[*i] && !ft_strchr("><", str[*i]) && !is_delim(&str[*i]))
// 		{
// 			quotes = 0;
// 			if (ft_strchr("'\"", str[*i]) && ft_strchr(&str[*i + 1], str[*i]))
// 			{
// 				quotes = str[*i];
// 				(*i)++;
// 			}
// 			(*i) += slice_next_part(&str[*i], arg, quotes);
// 			if (quotes)
// 				(*i)++;
// 		}
// 		if (arg && !errno)
// 		{
// 			*type = INPUT_OUTPUT;
// 			return (true);
// 		}
// 	}
// 	return (false);
// }